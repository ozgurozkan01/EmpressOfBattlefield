// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableActor.h"

#include "Treasure.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(GeometryCollection);
	
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GeometryCollection->SetNotifyBreaks(true);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->SetupAttachment(GetRootComponent());
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	
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
	SetLifeSpan(2.f);

	if (bIsBroken) { return; }
	
	if (GetWorld() && !Treasures.IsEmpty())
	{
		const int32 TreasureIndex = FMath::RandRange(0, Treasures.Num()-1);
		const FVector TreasureLocation = GetActorLocation() + FVector(0.f, 0.f, CapsuleComponent->GetScaledCapsuleHalfHeight()/2);
		GetWorld()->SpawnActor<ATreasure>(Treasures[TreasureIndex], TreasureLocation, GetActorRotation());
	}

	if (CapsuleComponent)
	{
		CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
		
	bIsBroken = true;
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (BreakSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BreakSound, ImpactPoint, FRotator::ZeroRotator);
	}
}
