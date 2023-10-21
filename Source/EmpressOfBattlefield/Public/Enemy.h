// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitInterface.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "Enemy.generated.h"

class AAIController;
class ASlashCharacter;
class UHealthBarComponent;
class UAttributeComponent;
class UUserWidget;
class UAnimMontage;

UCLASS()
class EMPRESSOFBATTLEFIELD_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void GetHit_Implementation(const FVector& ImpactPoint) override;	

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose;

protected:
	virtual void BeginPlay() override;
	
	void PlayHitReactionMontage(const FName& SectionName);
	void PlayDeathAnimMontage(const FName& SectionName);
	void Die(FName& SectionName);
	void ChangePatrolTarget();
	void MoveToTarget(TObjectPtr<AActor> Target);
	
	double CalculateHitLocationAngle(const FVector& ImpactPoint);
	FName DetermineWhichSideGetHit(const double& Theta);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	EDeathPose GetDeathPose(const FName& SectionName);
	bool ShouldChaseTarget(TObjectPtr<AActor> Target, float Radius);
	bool ShouldChangePatrolTarget(TObjectPtr<AActor> Target, float Radius);

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> CurrentTarget;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> PatrolTargetsContainer;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category=HUD)
	TSubclassOf<UUserWidget> HealthBarWidgetBlueprint;
	
	UPROPERTY(EditDefaultsOnly, Category="Animation | Montage")
	TObjectPtr<UAnimMontage> HitReactionMontage;

	UPROPERTY(EditDefaultsOnly, Category="Animation | Montage")
	TObjectPtr<UAnimMontage> RTLDeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category="Animation | Montage")
	TObjectPtr<UAnimMontage> UTDDeathAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditDefaultsOnly, Category="Particle Effect")
	TObjectPtr<UParticleSystem> HitParticle;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AAIController> AIController;
	
	UPROPERTY(EditInstanceOnly)
	double CombatRadius;

	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess = "true"))
	double MinPatrolRadius;

	UPROPERTY(EditInstanceOnly)
	int32 CurrentPatrolTargetIndex;

	bool ShouldMove;
};