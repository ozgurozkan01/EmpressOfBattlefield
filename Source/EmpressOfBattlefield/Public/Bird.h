// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;

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

	void MoveForward(float ScaleValue);
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Main")
	UCapsuleComponent *CapsuleCollider;

	UPROPERTY(EditDefaultsOnly, Category="Main")
	USkeletalMeshComponent *BirdMesh;
};
