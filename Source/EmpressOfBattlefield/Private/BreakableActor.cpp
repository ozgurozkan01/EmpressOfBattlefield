// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableActor.h"

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
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (BreakSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BreakSound, ImpactPoint, FRotator::ZeroRotator);
	}
}
