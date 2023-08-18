// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SlashAnimInstance.generated.h"

class ASlashCharacter;
class UCharacterMovementComponent;

UCLASS()
class EMPRESSOFBATTLEFIELD_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:

	virtual void NativeInitializeAnimation() override;;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	ASlashCharacter* SlashCharacter;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	UCharacterMovementComponent* SlashMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category=Movement)	
	float GroundSpeed;
};
