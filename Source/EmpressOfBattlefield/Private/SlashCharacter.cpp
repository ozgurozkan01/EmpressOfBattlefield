// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	
	// Player does not turn with mouse but camera does
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	CameraBoom->bUsePawnControlRotation = true; 

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character turns the movement direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 450.f, 0.f);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CurrentState = ECharacterState::ECS_Unequipped;
	CurrentAction = EActionState::EAS_Unoccupied;
}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("MainPlayer"));
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashMappingContext, 0);
		}
	}
}

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Movement);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(EquipWeaponAction, ETriggerEvent::Started, this, &ASlashCharacter::EquipWeapon);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
	}
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

	if (Controller == nullptr)
	{ return; }

	AddControllerPitchInput(LookDirection.Y);
	AddControllerYawInput(LookDirection.X);
}

void ASlashCharacter::Jump(const FInputActionValue& Value)
{
	const bool CanJump = Value.Get<bool>();
	
	if (CanJump && !GetCharacterMovement()->IsFalling())
	{
		ACharacter::Jump();
	}
}

void ASlashCharacter::EquipWeapon(const FInputActionValue& Value)
{
	const bool bEKeyPressed = Value.Get<bool>();

	AWeapon* OverlappedWeapon = Cast<AWeapon>(OverlappingItem);

	if(OverlappedWeapon && bEKeyPressed)
	{
		OverlappedWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		CurrentState = ECharacterState::ECS_EquippedOneHandWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappedWeapon;	
	}

	else if (bEKeyPressed && EquippedWeapon)
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			CurrentState = ECharacterState::ECS_Unequipped;
		}

		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CurrentState = ECharacterState::ECS_EquippedOneHandWeapon;
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
		CurrentAction = EActionState::EAS_Attacking;
		PlayAttackMontage();
	}
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

void ASlashCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::Arm()
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

void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharacter::AttackEnd()
{
	CurrentAction = EActionState::EAS_Unoccupied;
}
