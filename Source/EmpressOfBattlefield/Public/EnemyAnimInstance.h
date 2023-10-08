// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "Animation/AnimInstanceProxy.h"
#include "EnemyAnimInstance.generated.h"

class AEnemy;

UCLASS()
class EMPRESSOFBATTLEFIELD_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	void NativeInitializeAnimation() override;
protected:

	UPROPERTY(BlueprintReadOnly)
	AEnemy* Enemy;

	UPROPERTY(BlueprintReadWrite)
	EDeathPose DeathPose;

	FAnimInstanceProxy AnimInstanceProxy;
};
