// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashAnimInstance.h"
#include "SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());

	if (SlashCharacter)
	{
		SlashMovementComponent = SlashCharacter->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (SlashMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashMovementComponent->Velocity);
		IsFalling = SlashMovementComponent->IsFalling();
	}
}
