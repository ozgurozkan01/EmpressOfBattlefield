#include "SlashCharacter.h"

#include "AttributeComponent.h"
#include "Enemy.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "SlashAnimInstance.h"
#include "SlashHUD.h"
#include "SlashOverlay.h"
#include "Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 40.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator::ZeroRotator);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	EyeBrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	EyeBrows->SetupAttachment(GetMesh());
	EyeBrows->AttachmentName = FString("head");
	
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	
	// Player does not turn with mouse but camera does
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	CameraBoom->bUsePawnControlRotation = true;

	bCanRotateTarget = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character turns the movement direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 450.f, 0.f);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CurrentState = ECharacterState::ECS_Unequipped;
	CurrentAction = EActionState::EAS_Unoccupied;

	RotationInterpSpeed = 10.f;
}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("MainPlayer"));

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashMappingContext, 0);
		}
	}

	if (PlayerController)
	{
		TObjectPtr<ASlashHUD> SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());

		if (SlashHUD && AttributeComponent)
		{
			SlashOverlay = SlashHUD->GetSlashOverlay();

			if (SlashOverlay)
			{
				SlashOverlay->SetHealthBarPercentage(AttributeComponent->GetHealthPercentage());
				SlashOverlay->SetStaminaBarPercentage(1.f);
				SlashOverlay->SetGoldCount(0.f);
				SlashOverlay->SetSoulsCount(0.f);
			}
		}
	}
}

void ASlashCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCanRotateTarget)
	{
		FRotator LookAtRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), GetLookAtRotation(), DeltaSeconds, RotationInterpSpeed);
		SetActorRotation(LookAtRotation);	
	}
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	AnimInstance = Cast<USlashAnimInstance>(GetMesh()->GetAnimInstance());
	CurrentAction = EActionState::EAS_HitReaction;

	// If player gets hit, weapon collision cannot be deactivated.
	SetWeaponDamageBoxCollision(ECollisionEnabled::NoCollision);

	double Theta;

	if (Hitter)
	{
		Theta = CalculateHitLocationAngle(Hitter->GetActorLocation());
	}
	
	FName SectionName = DetermineWhichSideGetHit(Theta);

	if (IsAlive() && AnimInstance)
	{
		PlayHitReactionMontage(SectionName, AnimInstance);
	}
	
	else
	{
		Die(SectionName);
	}
	
	PlayEffects(ImpactPoint);
	SetWeaponDamageBoxCollision(ECollisionEnabled::NoCollision);
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Movement);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(EquipWeaponAction, ETriggerEvent::Started, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
	}
}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (AttributeComponent == nullptr) return 0.f;

	AttributeComponent->SetCurrentHealth(DamageAmount);
	
	if (AttributeComponent->GetHealthPercentage() <= 0.f)
	{
		TObjectPtr<AEnemy> CurrentEnemy = Cast<AEnemy>(EventInstigator->GetPawn());
		
		if (CurrentEnemy)
		{
			CurrentEnemy->Patrolling();
		}
	}
	
	if (SlashOverlay)
	{
		SlashOverlay->SetHealthBarPercentage(AttributeComponent->GetHealthPercentage());
	}

	return DamageAmount;
}

FRotator ASlashCharacter::GetLookAtRotation()
{
	if (CurrentTarget == nullptr || !CurrentTarget->ActorHasTag("Enemy")) { return GetActorRotation(); }

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTarget->GetActorLocation());
	LookAtRotation.Pitch = GetActorRotation().Pitch;
	LookAtRotation.Roll = GetActorRotation().Roll;

	return LookAtRotation;
}

void ASlashCharacter::Movement(const FInputActionValue& Value)
{
	if (CurrentAction != EActionState::EAS_Unoccupied) { return; }

	const FVector2D MovementDirection = Value.Get<FVector2D>();

	const FRotator ControllerRotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControllerRotation.Yaw, 0.f);

	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardVector, MovementDirection.Y);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightVector, MovementDirection.X);
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookDirection = Value.Get<FVector2D>();

	if (Controller == nullptr) { return; }
	AddControllerPitchInput(LookDirection.Y);
	AddControllerYawInput(LookDirection.X);
}

void ASlashCharacter::Jump(const FInputActionValue& Value)
{
	const bool CanJump = Value.Get<bool>();

	if (!CanJump || GetCharacterMovement()->IsFalling() || CurrentAction != EActionState::EAS_Unoccupied) { return; }

	ACharacter::Jump();
}

void ASlashCharacter::EKeyPressed(const FInputActionValue& Value)
{
	const bool bEKeyPressed = Value.Get<bool>();

	AWeapon* OverlappedWeapon = Cast<AWeapon>(OverlappingItem);

	if(OverlappedWeapon && bEKeyPressed)
	{
		EquipWeapon(OverlappedWeapon);
	}

	else if (bEKeyPressed && EquippedWeapon)
	{
		if (CanDisarm())
		{
			Disarm();
		}

		else if (CanArm())
		{
			Arm();
		}
		
		CurrentAction = EActionState::EAS_EquipWeapon;
	}
}
 
void ASlashCharacter::Attack(const FInputActionValue& Value)
{
	
	const bool bCanAttack = Value.Get<bool>() &&
							CurrentAction == EActionState::EAS_Unoccupied &&
							CurrentState != ECharacterState::ECS_Unequipped &&
							!GetCharacterMovement()->IsFalling();

	if (bCanAttack)
	{
		if (CurrentTarget != nullptr)
		{
			bCanRotateTarget = true;
		}
		CurrentAction = EActionState::EAS_Attacking;
		PlayAttackMontage();
	}
}

void ASlashCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
	CurrentState = ECharacterState::ECS_EquippedOneHandWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

bool ASlashCharacter::CanArm()
{
	return CurrentAction == EActionState::EAS_Unoccupied &&
		   CurrentState == ECharacterState::ECS_Unequipped &&
		   EquippedWeapon;
}

bool ASlashCharacter::CanDisarm()
{
	return CurrentAction == EActionState::EAS_Unoccupied &&
		   CurrentState != ECharacterState::ECS_Unequipped;
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CurrentState = ECharacterState::ECS_EquippedOneHandWeapon;
}

void ASlashCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CurrentState = ECharacterState::ECS_Unequipped;
}

void ASlashCharacter::Die(FName& SectionName)
{
	Tags.Add("Dead");
	PlayDeathMontage();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CurrentState = ECharacterState::ECS_Dead;
}

void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}


void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("WeaponSocket"));
	}
}

void ASlashCharacter::FinishedEquipping()
{
	CurrentAction = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::HitReactionEnd()
{
	CurrentAction = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	/*UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();*/
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharacter::AttackEnd()
{
	CurrentAction = EActionState::EAS_Unoccupied;
	bCanRotateTarget = false;
}

void ASlashCharacter::PlayDeathMontage()
{
	if (AnimInstance == nullptr) { return; }

	int32 SectionAmount = DeathMontage->GetNumSections();
	int32 SectionIndex = FMath::RandRange(0, SectionAmount-1);
	FName SectionName = DeathMontage->GetSectionName(SectionIndex);
	AnimInstance->Montage_Play(DeathMontage);
	AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	SetDeathPose(SectionName);
}

void ASlashCharacter::SetDeathPose(FName SectionName)
{
	if (SectionName == "Death1")
	{
		DeathPose = ESlashDeathPose::ESDP_Death1;
	}

	else
	{
		DeathPose = ESlashDeathPose::ESDP_Death2;
	}
}