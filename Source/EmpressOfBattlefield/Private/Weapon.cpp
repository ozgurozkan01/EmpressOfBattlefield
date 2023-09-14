// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "HitInterface.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EmpressOfBattlefield/DebugMacros.h"

AWeapon::AWeapon()
{
	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Box"));
	DamageBox->SetupAttachment(GetRootComponent());
	DamageBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	DamageBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Trace Start"));
	TraceStart->SetupAttachment(GetRootComponent());
	
	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Trace End"));
	TraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnDamageBoxOverlapBegin);
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

void AWeapon::OnDamageBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = TraceStart->GetComponentLocation();
	const FVector End = TraceEnd->GetComponentLocation();

	FHitResult BoxHit;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(2.5f, 2.5f, 2.5f),
		TraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		IgnoredActors,
		EDrawDebugTrace::None,
		BoxHit,
		true
		);

	if (BoxHit.GetActor())
	{
		// if hit object inherites from IHitInterface, then call the methods,
		// But this method belongs to hit objects own. So we do not need to cast
		// specific classes like actor, character etc.
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint);
		}

		IgnoredActors.AddUnique(BoxHit.GetActor());
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AttachmentTransformRules, InSocketName);	
}
