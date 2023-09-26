// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "HitInterface.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Field/FieldSystemComponent.h"
#include "Particles/ParticleSystemComponent.h"

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

	FieldSystemComponent = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("Field System Component"));
	FieldSystemComponent->SetupAttachment(GetRootComponent());
	
	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("Radial Falloff"));
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("Radial Vector"));
	MetaDataFilter = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("Meta Data Filter"));
	
	FalloffMagnitude = 1500000000.f;
	VectorMagnitude = 50000000000.f;
	SphereRadius = 200.f;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	MetaDataFilter->ObjectType = EFieldObjectType::Field_Object_Destruction;
	MetaDataFilter->FilterType = EFieldFilterType::Field_Filter_Dynamic;
	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnDamageBoxOverlapBegin);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	IsItemTaken = true;
	AttachMeshToSocket(InParent, InSocketName);

	if (UnpickUpParticle)
	{
		UnpickUpParticle->Deactivate();
	}
	
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
	
	if (SphereCollider)
	{
		SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::CreateFields(const FVector& FieldLocation)
{
	// Min and Max Range is just a number which are multiplied by the Force.
	// So before the determine these values, watch out that.
	RadialFalloff->SetRadialFalloff(FalloffMagnitude,0.8f, 1.f, 0.f, SphereRadius, FieldLocation, EFieldFalloffType::Field_FallOff_None);	
	RadialVector->SetRadialVector(VectorMagnitude, FieldLocation);

	if (FieldSystemComponent)
	{
		if (RadialFalloff)
		{
			FieldSystemComponent->ApplyPhysicsField(true, Field_ExternalClusterStrain, nullptr, RadialFalloff);
		}

		if (RadialVector)
		{
			FieldSystemComponent->ApplyPhysicsField(true, Field_LinearForce, MetaDataFilter, RadialVector);
		}
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
			HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
		}

		IgnoredActors.AddUnique(BoxHit.GetActor());
		CreateFields(BoxHit.ImpactPoint);
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AttachmentTransformRules, InSocketName);	
}
