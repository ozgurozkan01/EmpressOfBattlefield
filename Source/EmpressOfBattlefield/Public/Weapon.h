// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "CharacterTypes.h"
#include "Weapon.generated.h"

class UFieldSystemComponent;
class UFieldSystemMetaDataFilter;
class URadialVector;
class URadialFalloff;
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
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void CreateFields(const FVector& FieldLocation);
	
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
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(VisibleAnywhere, Category="Damage")
	TObjectPtr<UBoxComponent> DamageBox;

	UPROPERTY(VisibleAnywhere, Category="Trace")
	TObjectPtr<USceneComponent> TraceStart;
	
	UPROPERTY(VisibleAnywhere, Category="Trace")
	TObjectPtr<USceneComponent> TraceEnd;

	UPROPERTY(EditAnywhere, Category=Damage)
	float Damage;
	
	// Field System Variables

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFieldSystemComponent> FieldSystemComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URadialFalloff> RadialFalloff;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URadialVector> RadialVector;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFieldSystemMetaDataFilter> MetaDataFilter;
	
	UPROPERTY(EditAnywhere)
	float FalloffMagnitude;

	UPROPERTY(EditAnywhere)
	float VectorMagnitude;
	
	UPROPERTY(EditAnywhere)
	float SphereRadius;
};
