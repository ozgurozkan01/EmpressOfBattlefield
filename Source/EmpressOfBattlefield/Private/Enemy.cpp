#include "Enemy.h"

#include "AIController.h"
#include "AttributeComponent.h"
#include "EnemyAnimInstance.h"
#include "Engine/EngineTypes.h"
#include "HealthBar.h"
#include "HealthBarComponent.h"
#include "SlashCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	PawnSensingComponent->SightRadius = 1250.f;
	PawnSensingComponent->SetPeripheralVisionAngle(60.f);

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic); // Weapon does not overlap with Pawns
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // Weapon trace channel
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // prevent breakdown of player's camera when collide with enemy
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // same
	GetMesh()->SetGenerateOverlapEvents(true); // Activate overlap event generation

	// UActorComponent does not need to attach to the any component
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute Component"));

	HealthBarWidgetComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar Component"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 125.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	DeathPose = EDeathPose::EDP_Alive;
	EnemyState = EEnemyState::EES_Patroling;
	
	CombatRadius = 1000.f;
	AttackRadius = 100.f;
	MinPatrolRadius = 200.f;
	CurrentTarget = 0;
	CurrentPatrolTargetIndex = 1;
	PatrolWaitRate = 2.f;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Enemy"));
	
	AIController = Cast<AAIController>(GetController());

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::SeePawn);
		PawnSensingComponent->OnHearNoise.AddDynamic(this, &AEnemy::HearNoise);
	}
	
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetWidgetClass(HealthBarWidgetBlueprint);
		HealthBarWidgetComponent->SetHealthPercent(AttributeComponent->GetHealthPercentage());
		HealthBarWidgetComponent->SetVisibility(false);
	}
	
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, PatrolWaitRate);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckCurrentTarget();
}

bool AEnemy::ShouldChangePatrolTarget(TObjectPtr<AActor> Target, float Radius)
{
	if (Target == nullptr) { return false; }
	
	float Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
	return Distance <= Radius;
}

void AEnemy::ChangePatrolTarget()
{
	CurrentTarget = PatrolTargetsContainer[CurrentPatrolTargetIndex];
	CurrentPatrolTargetIndex++;

	if (CurrentPatrolTargetIndex >= PatrolTargetsContainer.Num())
	{
		CurrentPatrolTargetIndex = 0;
	}
}

void AEnemy::MoveToTarget(TObjectPtr<AActor> Target)
{
	if (AIController == nullptr || Target == nullptr) { return; }
	
	FAIMoveRequest MoveRequest; 
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	AIController->MoveTo(MoveRequest);
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(CurrentTarget);
}

void AEnemy::CheckCurrentTarget()
{
	if (!InTargetRange(CurrentTarget, CombatRadius) && EnemyState == EEnemyState::EES_Chasing)
	{
		if (HealthBarWidgetComponent)
		{
			HealthBarWidgetComponent->SetVisibility(false);
		}
		
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		CurrentTarget = PatrolTargetsContainer[CurrentPatrolTargetIndex];
		EnemyState = EEnemyState::EES_Patroling;
		MoveToTarget(CurrentTarget);
	}

	else if(ShouldChangePatrolTarget(CurrentTarget, MinPatrolRadius) && EnemyState == EEnemyState::EES_Patroling)
	{
		UE_LOG(LogTemp, Warning, TEXT("Patrol Target!"));
		ChangePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 2.2f);
	}

	else if (InTargetRange(CurrentTarget, AttackRadius) && CurrentTarget->ActorHasTag("MainPlayer") && EnemyState != EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Attacking;
		/*FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTarget->GetActorLocation());
		SetActorRotation(LookAtRotation);*/
		UE_LOG(LogTemp, Warning, TEXT("Attack!"));
	}

	else if (!InTargetRange(CurrentTarget, AttackRadius) && CurrentTarget->ActorHasTag("MainPlayer") && EnemyState == EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Chasing;
		UE_LOG(LogTemp, Warning, TEXT("Chasing!"));
		MoveToTarget(CurrentTarget);
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::SeePawn(APawn* SeenPawn)
{
	if (!InTargetRange(CurrentTarget, AttackRadius) && SeenPawn->ActorHasTag(FName("MainPlayer")) && EnemyState != EEnemyState::EES_Chasing)
	{
		EnemyState = EEnemyState::EES_Chasing;
		UE_LOG(LogTemp, Warning, TEXT("Pawn Seen!"));
		CurrentTarget = SeenPawn;
		MoveToTarget(CurrentTarget);
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
	}
}

void AEnemy::HearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	double Theta = CalculateHitLocationAngle(ImpactPoint);
	FName SectionName = DetermineWhichSideGetHit(Theta);

	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(true);
	}
	
	if (AttributeComponent && AttributeComponent->IsAlive())
	{
		PlayHitReactionMontage(SectionName);
	}

	else
	{
		DeathPose = GetDeathPose(SectionName);
		Die(SectionName);
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, ImpactPoint);
	}	
}

void AEnemy::PlayHitReactionMontage(const FName& SectionName)
{
		TObjectPtr<UEnemyAnimInstance> AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	
	if (AnimInstance && HitReactionMontage)
	{
		AnimInstance->Montage_Play(HitReactionMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactionMontage);
	}
}

void AEnemy::PlayDeathAnimMontage(const FName& SectionName)
{
	TObjectPtr<UEnemyAnimInstance> AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	TObjectPtr<ASlashCharacter> SlashCharacter = Cast<ASlashCharacter>(CurrentTarget);
	
	if (AnimInstance == nullptr && SlashCharacter == nullptr) { return; }
	
	if (SlashCharacter)
	{
		AnimInstance->AttackType = SlashCharacter->AttackType;
	}
	
	if (SlashCharacter && SlashCharacter->AttackType == EAttackType::EAT_RightToLeft && RTLDeathAnimMontage)
	{
		AnimInstance->Montage_Play(RTLDeathAnimMontage);
		AnimInstance->Montage_JumpToSection(SectionName, RTLDeathAnimMontage);
	}

	else
	{
		AnimInstance->Montage_Play(UTDDeathAnimMontage);
		AnimInstance->Montage_JumpToSection(SectionName, UTDDeathAnimMontage);
	}
	
}

double AEnemy::CalculateHitLocationAngle(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector LoweredHit(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (LoweredHit - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);
	
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if (CrossProduct.Z < 0)
	{
		Theta *= -1;
	}

	return Theta;
}

FName AEnemy::DetermineWhichSideGetHit(const double& Theta)
{
	FName Section("FromBack");
	
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}

	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}

	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	return Section;
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (AttributeComponent &&
		HealthBarWidgetComponent &&
		HealthBarWidgetComponent->GetHealthBarWidget())
	{
		AttributeComponent->SetCurrentHealth(DamageAmount);
		HealthBarWidgetComponent->SetHealthPercent(AttributeComponent->GetHealthPercentage());
		HealthBarWidgetComponent->GetHealthBarWidget()->SetHealthBarColor(DamageAmount);
		CurrentTarget = Cast<ASlashCharacter>(EventInstigator->GetPawn());

		if (CurrentTarget)
		{
			EnemyState = EEnemyState::EES_Attacking;
			MoveToTarget(CurrentTarget);
		}
	}
	return DamageAmount;
}

EDeathPose AEnemy::GetDeathPose(const FName& SectionName)
{
	if (SectionName == "FromBack")
	{
		return EDeathPose::EDP_DeathToFront;
	}

	if (SectionName == "FromFront")
	{
		return EDeathPose::EDP_DeathToBack;
	}

	if (SectionName == "FromRight")
	{
		return EDeathPose::EDP_DeathToLeft;
	}

	return EDeathPose::EDP_DeathToRight;
}

bool AEnemy::InTargetRange(TObjectPtr<AActor> Target, float Radius)
{
	if (Target == nullptr) { return false; }
	
	const float Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
	return Distance <= Radius;
}

void AEnemy::Die(FName& SectionName)
{
	PlayDeathAnimMontage(SectionName);

	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(false);
	}
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f); // Destroy actor after 5 seconds.
}