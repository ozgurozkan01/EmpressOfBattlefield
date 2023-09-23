// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class UGroomComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class EMPRESSOFBATTLEFIELD_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterCurrentState() const { return CurrentState; }
	
	UPROPERTY(VisibleAnywhere)
	ECharacterState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	EActionState CurrentAction;
	
protected:
	virtual void BeginPlay() override;

	// Callbacks for input
	void Movement(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void EquipWeapon(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	
	bool CanArm();
	bool CanDisarm();
	
	
	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishedEquipping();

	UFUNCTION(BlueprintCallable)
	void SetWeaponDamageBoxCollision(ECollisionEnabled::Type CollisionEnabled);
	
	// Play Montage Functions
	void PlayAttackMontage();
	void PlayEquipMontage(const FName& SectionName);
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
public:

	UPROPERTY(EditDefaultsOnly, Category=Camera)
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(EditDefaultsOnly, Category=Camera)
	UCameraComponent* Camera;
	
	UPROPERTY(EditDefaultsOnly, Category="Input | Map")
	UInputMappingContext* SlashMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Input | Action")	
	UInputAction* MovementAction;

	UPROPERTY(EditDefaultsOnly, Category="Input | Action")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category="Input | Action")
	UInputAction* JumpAction;	

	UPROPERTY(EditDefaultsOnly, Category="Input | Action")
	UInputAction* EquipWeaponAction;		

	UPROPERTY(EditDefaultsOnly, Category="Input | Action")
	UInputAction* AttackAction;		
	
	// Groom Component is used for the hair, eyebrows or etc.
	UPROPERTY(EditDefaultsOnly, Category=Hair)
	UGroomComponent* Hair;

	UPROPERTY(EditDefaultsOnly, Category=Hair)
	UGroomComponent* EyeBrows;

	UPROPERTY(VisibleInstanceOnly, Category=Item)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere, Category="Item | Weapon")
	AWeapon* EquippedWeapon;
	
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* EquipMontage;

	UPROPERTY(VisibleAnywhere, Category=Montage)
	float AttackMontageSpeedRate;
};
