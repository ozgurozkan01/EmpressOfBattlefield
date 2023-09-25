// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"

#include "SlashCharacter.h"
#include "Kismet/GameplayStatics.h"

ATreasure::ATreasure()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATreasure::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AddActorWorldRotation(FRotator(0.f,  0.01 * DeltaSeconds, 0.f));
}

void ATreasure::OverlappingBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);

		if (SlashCharacter)
		{
			if (PickupSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this,PickupSound, GetActorLocation());
			}

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Cyan, FString::Printf(TEXT("%d"), GoldCount));
			}
			
			Destroy();
		}
	}
}
