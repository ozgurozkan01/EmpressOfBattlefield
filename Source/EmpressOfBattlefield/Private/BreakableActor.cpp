// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableActor.h"

#include "Treasure.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(GeometryCollection);
	
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetNotifyBreaks(true);

	bIsBroken = false;
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();

	if (GeometryCollection)
	{
		GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::BreakActorActive);
	}
}

void ABreakableActor::BreakActorActive(const FChaosBreakEvent& BreakEvent)
{
	SetLifeSpan(4.f);


	if (!bIsBroken)
	{
		if (GetWorld() && Treasure)
		{
			GetWorld()->SpawnActor<ATreasure>(Treasure, GetActorLocation() + FVector(0.f, 0.f, 50.f), GetActorRotation());
		}	

		bIsBroken = true;
	}
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (BreakSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BreakSound, ImpactPoint, FRotator::ZeroRotator);
	}
}
