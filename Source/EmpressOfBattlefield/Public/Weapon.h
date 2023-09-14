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
	
	FORCEINLINE UBoxComponent* GetDamageBox() const { return DamageBox; }
	
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);
	void Equip(USceneComponent* InParent, FName InSocketName);
	
	UPROPERTY()
	TArray<AActor*> IgnoredActors;
protected:

	virtual void BeginPlay() override;

	void OverlappingBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	void OverlappingEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnDamageBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResul);
private:

	UPROPERTY(EditDefaultsOnly, Category="Equip Sound")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category="Damage")
	UBoxComponent* DamageBox;

	UPROPERTY(VisibleAnywhere, Category="Trace")
	USceneComponent* TraceStart;
	
	UPROPERTY(VisibleAnywhere, Category="Trace")
	USceneComponent* TraceEnd;

};
