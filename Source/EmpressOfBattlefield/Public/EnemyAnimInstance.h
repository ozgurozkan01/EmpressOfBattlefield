// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "EnemyAnimInstance.generated.h"

class AEnemy;
class UCharacterMovementComponent;
UCLASS()
class EMPRESSOFBATTLEFIELD_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	void NativeInitializeAnimation() override;

	void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadOnly)
	EAttackType AttackType;
protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AEnemy> Enemy;

	UPROPERTY(BlueprintReadWrite)
	EDeathPose DeathPose;

	UPROPERTY(BlueprintReadWrite)
	EEnemyState EnemyState;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> MovementController;
};
