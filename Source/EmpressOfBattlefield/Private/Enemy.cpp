#include "Enemy.h"

#include "AttributeComponent.h"
#include "HealthBar.h"
#include "HealthBarComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

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

	DeathPose = EDeathPose::EDP_Alive;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidgetComponent)
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Cyan, "HealthBarComponent Valid");
		HealthBarWidgetComponent->SetWidgetClass(HealthBarWidgetBlueprint);
		HealthBarWidgetComponent->SetHealthPercent(AttributeComponent->GetHealthPercentage());
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	double Theta = CalculateHitLocationAngle(ImpactPoint);
	FName SectionName = DetermineWhichSideGetHit(Theta);

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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactionMontage)
	{
		AnimInstance->Montage_Play(HitReactionMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactionMontage);
	}
}

void AEnemy::PlayDeathAnimMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && DeathAnimMontage)
	{
		AnimInstance->Montage_Play(DeathAnimMontage);
		AnimInstance->Montage_JumpToSection(SectionName, DeathAnimMontage);
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

void AEnemy::Die(FName& SectionName)
{
	PlayDeathAnimMontage(SectionName);
}
