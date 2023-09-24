// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Treasure.generated.h"

class USoundBase;

UCLASS()
class EMPRESSOFBATTLEFIELD_API ATreasure : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OverlappingBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:

	UPROPERTY(EditDefaultsOnly, Category=Sound)
	USoundBase* PickupSound;
};
