// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitInterface.h"
#include "GameFramework/Actor.h"
#include "BreakableActor.generated.h"

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
	void GetHit_Implementation(const FVector& ImpactPoint) override;

	UFUNCTION()
	virtual void BreakActorActive(const FChaosBreakEvent& BreakEvent);

private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

public:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* BreakSound;

};