// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitInterface.h"
#include "GameFramework/Actor.h"
#include "BreakableActor.generated.h"

class UCapsuleComponent;
class ATreasure;
struct FChaosBreakEvent;
class UGeometryCollectionComponent;
class USoundBase;

UCLASS()
class EMPRESSOFBATTLEFIELD_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();

protected:
	virtual void BeginPlay() override;
	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	UFUNCTION()
	virtual void BreakActorActive(const FChaosBreakEvent& BreakEvent);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
public:
	UPROPERTY(EditDefaultsOnly, Category=Sound)
	TObjectPtr<USoundBase> BreakSound;

	UPROPERTY(EditDefaultsOnly, Category=Treasure)
	TArray<TSubclassOf<ATreasure>> Treasures;

	UPROPERTY(VisibleAnywhere)
	bool bIsBroken;
};