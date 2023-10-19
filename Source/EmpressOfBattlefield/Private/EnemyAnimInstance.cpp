// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Enemy == nullptr)
	{
		Enemy = Cast<AEnemy>(TryGetPawnOwner());
	}

	if (Enemy)
	{
		DeathPose = Enemy->DeathPose;
		MovementController = Enemy->GetCharacterMovement();
	}

}

void UEnemyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (Enemy)
	{
		DeathPose = Enemy->DeathPose;
		GroundSpeed = MovementController->Velocity.Length();
	}
}
