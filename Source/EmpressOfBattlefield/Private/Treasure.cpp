// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"

#include "SlashCharacter.h"
#include "Kismet/GameplayStatics.h"

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
			Destroy();
		}
	}
}
