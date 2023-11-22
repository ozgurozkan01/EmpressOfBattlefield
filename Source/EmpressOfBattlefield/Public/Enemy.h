// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Enemy.generated.h"

class UEnemyAnimInstance;
class AAIController;
class ASlashCharacter;
class UHealthBarComponent;
class UUserWidget;
class UAnimMontage;
class UPawnSensingComponent;
class UPhysicsAsset;

UCLASS()
class EMPRESSOFBATTLEFIELD_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SeePawn(APawn* SeenPawn);
	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;	
	void AttackEnd() override;
	void Patrolling();
	void ClearTimerHandle(FTimerHandle& Timer);

	UPROPERTY(BlueprintReadOnly)
	EEnemyDeathPose DeathPose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState;
protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override; // When actor is destroyed then this function runs.
	
	void Attack();
	void PlayDeathAnimMontage(const FName& SectionName);
	void Die(FName& SectionName) override;
	void ChangePatrolTarget();
	void MoveToTarget(TObjectPtr<AActor> Target);
	void PatrolTimerFinished();
	void CheckCurrentTarget();
	void ChasingTarget();
	void StartAttackTimer();
	void SetHealthWidgetInitialProperties();
	void SetHealthBarVisibility(bool bIsVisible);
	void AttachDefaultWeaponAtStart();
	
	bool InTargetRange(TObjectPtr<AActor> Target, float Radius);
	bool ShouldChangePatrolTarget(TObjectPtr<AActor> Target, float Radius);
	bool IsInsideAttackRadius();
	bool IsInsideCombatRadius();
	bool CanChangePatrolTarget();
	bool CanChaseTarget();
	bool CanAttack();
	bool CanPatrol();

	EEnemyDeathPose GetDeathPose(const FName& SectionName);

private:

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> PatrolTargetsContainer;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category=HUD)
	TSubclassOf<UUserWidget> HealthBarWidgetBlueprint;
	
	UPROPERTY(EditDefaultsOnly, Category=HUD)
	TSubclassOf<AWeapon> WeaponClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Animation | Montage")
	TObjectPtr<UAnimMontage> RTLDeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category="Animation | Montage")
	TObjectPtr<UAnimMontage> UTDDeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<UEnemyAnimInstance> AnimInstance;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AAIController> AIController;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;
	
	UPROPERTY(EditInstanceOnly)
	double CombatRadius;

	UPROPERTY(EditAnywhere)
	double AttackRadius;
	
	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess = "true"))
	double MinPatrolRadius;

	UPROPERTY(EditInstanceOnly)
	int32 CurrentPatrolTargetIndex;
	
	UPROPERTY(EditAnywhere, Category=Speed)
	float PatrollingSpeed;

	UPROPERTY(EditAnywhere, Category=Speed)
	float ChasingSpeed;

	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;
	
	float PatrolWaitRate;
	float AttackWaitRate;
};