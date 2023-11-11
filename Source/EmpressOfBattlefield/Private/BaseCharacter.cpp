#include "BaseCharacter.h"
#include "AttributeComponent.h"
#include "Components//BoxComponent.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "MotionWarpingComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// UActorComponent does not need to attach to the any component
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute Component"));
	AttackMontageSpeedRate = 1.25f;
	
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AttackMontage)
	{
		AttackMontage->BlendIn = 0.f;
	}
}

void ABaseCharacter::Die(FName& SectionName)
{
}

double ABaseCharacter::CalculateHitLocationAngle(const FVector& ImpactPoint)
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

FName ABaseCharacter::DetermineWhichSideGetHit(const double& Theta)
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

void ABaseCharacter::AttackEnd()
{
	
}

void ABaseCharacter::PlayHitReactionMontage(const FName& SectionName, UAnimInstance* AnimInstance)
{
	if (AnimInstance && HitReactionMontage)
	{
		AnimInstance->Montage_Play(HitReactionMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactionMontage);
	}
}

void ABaseCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AttackMontage || AttackMontage->GetNumSections()  == 0 || !AnimInstance) { return; }

	AnimInstance->Montage_Play(AttackMontage, AttackMontageSpeedRate);
	const int32 SectionNum = AttackMontage->GetNumSections();
	const int32 Selection = FMath::RandRange(0, SectionNum-1);
	FName SectionName = AttackMontage->GetSectionName(Selection);;
	AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
}

void ABaseCharacter::PlayEffects(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, ImpactPoint);
	}	
}

bool ABaseCharacter::IsAlive()
{
	return AttributeComponent && AttributeComponent->IsAlive();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetWeaponDamageBoxCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetDamageBox())
	{
		EquippedWeapon->GetDamageBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoredActors.Empty();
	}
}
