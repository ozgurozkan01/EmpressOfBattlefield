#include "Enemy.h"

#include "AIController.h"
#include "Weapon.h"
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
	
	HealthBarWidgetComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar Component"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	EnemyState = EEnemyState::EES_Patroling;
	
	CombatRadius = 1000.f;
	AttackRadius = 100.f;
	MinPatrolRadius = 200.f;
	CurrentTarget = 0;
	CurrentPatrolTargetIndex = 1;
	PatrolWaitRate = 2.f;
	AttackWaitRate = 1.25;
	PatrollingSpeed = 125.f;
	ChasingSpeed = 350.f;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Enemy"));
	
	AIController = Cast<AAIController>(GetController());

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::SeePawn);
	}
	
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetWidgetClass(HealthBarWidgetBlueprint);
		HealthBarWidgetComponent->SetHealthPercent(AttributeComponent->GetHealthPercentage());
		HealthBarWidgetComponent->SetVisibility(false);
	}

	if (GetWorld())
	{
		TObjectPtr<AWeapon> DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = DefaultWeapon;	
	}
	
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, PatrolWaitRate);
}

void AEnemy::Attack()
{
	PlayAttackMontage();
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

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CurrentTarget, AttackRadius);
}

bool AEnemy::IsInsideCombatRadius()
{
	return InTargetRange(CurrentTarget, CombatRadius);
}

bool AEnemy::CanChangePatrolTarget()
{
	return ShouldChangePatrolTarget(CurrentTarget, MinPatrolRadius) && EnemyState == EEnemyState::EES_Patroling;
}

bool AEnemy::CanChaseTarget()
{
	return !IsInsideAttackRadius() && CurrentTarget->ActorHasTag("MainPlayer") && EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::CanAttack()
{
	return IsInsideAttackRadius() && CurrentTarget->ActorHasTag("MainPlayer") && EnemyState != EEnemyState::EES_Attacking;
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
	if (!IsInsideCombatRadius() && EnemyState == EEnemyState::EES_Chasing)
	{
		if (HealthBarWidgetComponent)
		{
			HealthBarWidgetComponent->SetVisibility(false);
		}

		Patrolling();
	}

	else if(CanChangePatrolTarget())
	{
		ChangePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, PatrolWaitRate);
	}

	else if (CanAttack())
	{
		StartAttackTimer();
	}

	else if (CanChaseTarget())
	{
		ChasingTarget();
	}
}

void AEnemy::Patrolling()
{
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	CurrentTarget = PatrolTargetsContainer[CurrentPatrolTargetIndex];
	EnemyState = EEnemyState::EES_Patroling;
	MoveToTarget(CurrentTarget);
}

void AEnemy::ChasingTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	MoveToTarget(CurrentTarget);
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
}

void AEnemy::ClearTimerHandle(FTimerHandle& Timer)
{
	GetWorldTimerManager().ClearTimer(Timer);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackWaitRate);
}

void AEnemy::SeePawn(APawn* SeenPawn)
{
	if (EnemyState == EEnemyState::EES_Chasing) { return; }
	
	if (!IsInsideAttackRadius() && SeenPawn->ActorHasTag(FName("MainPlayer")))
	{
		ClearTimerHandle(PatrolTimer);
		CurrentTarget = SeenPawn;
		ChasingTarget();
	}
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

		/*if (CurrentTarget)
		{
			//EnemyState = EEnemyState::EES_Attacking;
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CurrentTarget);
		}*/
		ChasingTarget();
	}
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	Super::Destroyed();

	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
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