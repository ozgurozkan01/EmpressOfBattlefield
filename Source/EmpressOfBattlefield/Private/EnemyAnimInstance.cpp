// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

#include "Enemy.h"
#include "PropertyAccess.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Enemy == nullptr)
	{
		Enemy = Cast<AEnemy>(TryGetPawnOwner());
		return;
	}

	DeathPose = Enemy->DeathPose;
}

void UEnemyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (Enemy)
	{
		DeathPose = Enemy->DeathPose;
	}
}
