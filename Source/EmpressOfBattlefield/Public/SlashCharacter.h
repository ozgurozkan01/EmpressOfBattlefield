// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class USlashAnimInstance;
class UGroomComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class EMPRESSOFBATTLEFIELD_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterCurrentState() const { return CurrentState; }
	
	UPROPERTY(VisibleAnywhere)
	ECharacterState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	EActionState CurrentAction;

	UPROPERTY(VisibleAnywhere)
	EAttackType AttackType;
	
protected:
	virtual void BeginPlay() override;

	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	
	// Callbacks for input
	void Movement(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void EKeyPressed(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

	void EquipWeapon(AWeapon* Weapon);
	
	bool CanArm();
	bool CanDisarm();
	void Arm();
	void Disarm();

	
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishedEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactionEnd();	
	
	// Play Montage Functions
	void PlayEquipMontage(const FName& SectionName);
	
	virtual void AttackEnd() override;
public:

	UPROPERTY(EditDefaultsOnly, Category=Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(EditDefaultsOnly, Category=Camera)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditDefaultsOnly, Category="Input | Map")
	TObjectPtr<UInputMappingContext> SlashMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Input | Action")	
	TObjectPtr<UInputAction> MovementAction;

	UPROPERTY(EditDefaultsOnly, Category="Input | Action")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category="Input | Action")
	TObjectPtr<UInputAction> JumpAction;	

	UPROPERTY(EditDefaultsOnly, Category="Input | Action")
	TObjectPtr<UInputAction> EquipWeaponAction;		

	UPROPERTY(EditDefaultsOnly, Category="Input | Action")
	TObjectPtr<UInputAction> AttackAction;		
	
	// Groom Component is used for the hair, eyebrows or etc.
	UPROPERTY(EditDefaultsOnly, Category=Hair)
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(EditDefaultsOnly, Category=Hair)
	TObjectPtr<UGroomComponent> EyeBrows;

	UPROPERTY(VisibleInstanceOnly, Category=Item)
	TObjectPtr<AItem> OverlappingItem;
	
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<USlashAnimInstance> AnimInstance;
};
