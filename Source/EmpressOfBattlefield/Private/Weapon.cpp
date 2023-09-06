// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Box"));
	DamageBox->SetupAttachment(GetRootComponent());
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	IsItemTaken = true;
	AttachMeshToSocket(InParent, InSocketName);

	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
	
	if (SphereCollider)
	{
		SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::OverlappingBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OverlappingBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OverlappingEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OverlappingEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AttachmentTransformRules, InSocketName);	
}
