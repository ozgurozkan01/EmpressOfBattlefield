// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "SlashAnimInstance.generated.h"

class ASlashCharacter;
class UCharacterMovementComponent;

UCLASS()
class EMPRESSOFBATTLEFIELD_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:

	// as BeginPlay()
	virtual void NativeInitializeAnimation() override;;

	// as Tick()
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category="Movement | Character State")
	ECharacterState CharacterState;
	
	UPROPERTY(BlueprintReadOnly)
	ASlashCharacter* SlashCharacter;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	UCharacterMovementComponent* SlashMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category=Movement)	
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category=Movement)	
	bool IsFalling;
};
