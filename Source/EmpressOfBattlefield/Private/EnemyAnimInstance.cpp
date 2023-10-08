// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

#include "Enemy.h"

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