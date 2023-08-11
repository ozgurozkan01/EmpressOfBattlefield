// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;

UCLASS()
class EMPRESSOFBATTLEFIELD_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Main")
	UCapsuleComponent *CapsuleCollider;

	UPROPERTY(EditDefaultsOnly, Category="Main")
	USkeletalMeshComponent *BirdMeshComponent;

	UPROPERTY()
	UAnimSequence* BirdFlyAnimation;
	
	UPROPERTY(EditDefaultsOnly, Category="Main")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category="Main")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* BirdMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MoveAction;
	
};
