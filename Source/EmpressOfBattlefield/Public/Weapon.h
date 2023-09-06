// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
class USoundBase;

UCLASS()
class EMPRESSOFBATTLEFIELD_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();
	
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);
	void Equip(USceneComponent* InParent, FName InSocketName);

protected:
	void OverlappingBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	void OverlappingEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:

	UPROPERTY(EditDefaultsOnly, Category="Equip Sound")
	USoundBase* EquipSound;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	UBoxComponent* DamageBox;
};
