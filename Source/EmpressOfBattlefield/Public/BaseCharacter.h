// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class UAttributeComponent;
class UMotionWarpingComponent;

UCLASS()
class EMPRESSOFBATTLEFIELD_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponDamageBoxCollision(ECollisionEnabled::Type CollisionEnabled);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> CurrentTarget;
protected:

	/**	Virtual Functions */
	virtual void BeginPlay() override;
	virtual void Die(FName& SectionName);
	virtual void PlayAttackMontage();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	
	double CalculateHitLocationAngle(const FVector& ImpactPoint);
	FName DetermineWhichSideGetHit(const double& Theta);
	
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWarpTranslation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWarpRotation();
	
	void PlayHitReactionMontage(const FName& SectionName, UAnimInstance* AnimInstance);
	void PlayEffects(const FVector& ImpactPoint);
	
	bool IsAlive();
	
	UPROPERTY(VisibleAnywhere, Category="Item | Weapon")
	TObjectPtr<AWeapon> EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	
private:
	
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditDefaultsOnly, Category="Particle Effect")
	TObjectPtr<UParticleSystem> HitParticle;
	
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Animation | Montage")
	TObjectPtr<UAnimMontage> HitReactionMontage;

	UPROPERTY(VisibleAnywhere, Category=Montage)
	float AttackMontageSpeedRate;

	UPROPERTY(VisibleAnywhere, Category=Montage)
	float WarpingDistance;
};
