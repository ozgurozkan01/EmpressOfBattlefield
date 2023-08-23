// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "SlashCharacter.h"

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	IsItemTaken = true;
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AttachmentTransformRules, InSocketName);	
}

void AWeapon::OverlappingBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OverlappingBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OverlappingEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OverlappingEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}