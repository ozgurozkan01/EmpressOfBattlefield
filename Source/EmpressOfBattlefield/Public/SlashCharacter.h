// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SlashCharacter.generated.h"

class UGroomComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class AItem;

UCLASS()
class EMPRESSOFBATTLEFIELD_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

protected:
	virtual void BeginPlay() override;
	
	void Movement(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void EquipWeapon(const FInputActionValue& Value);
public:

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	UCameraComponent* Camera;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* SlashMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")	
	UInputAction* MovementAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* JumpAction;	

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* EquipWeaponAction;		
	
	// Groom Component is used for the hair, eyebrows or etc.
	UPROPERTY(EditDefaultsOnly, Category=Hair)
	UGroomComponent* Hair;

	UPROPERTY(EditDefaultsOnly, Category=Hair)
	UGroomComponent* EyeBrows;

	UPROPERTY(VisibleInstanceOnly, Category="Item")
	AItem* OverlappingItem;
	
};
