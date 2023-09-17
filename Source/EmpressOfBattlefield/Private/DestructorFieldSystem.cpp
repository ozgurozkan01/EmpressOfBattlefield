// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructorFieldSystem.h"
#include "Field/FieldSystemComponent.h"

ADestructorFieldSystem::ADestructorFieldSystem()
{
	FalloffMagnitude = 1500000.f;
	VectorMagnitude = 50000000.f;
	SphereRadius = 200.f;
	
	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("Radial Falloff"));
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("Radial Vector"));
	MetaDataFilter = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("Meta Data Filter"));
}

void ADestructorFieldSystem::BeginPlay()
{
	Super::BeginPlay();
	MetaDataFilter->ObjectType = EFieldObjectType::Field_Object_Destruction;
	MetaDataFilter->FilterType = EFieldFilterType::Field_Filter_Dynamic;
	// Min and Max Range is just a number which are multiplied by the Force.
	// So before the determine these values, watch out that.
	RadialFalloff->SetRadialFalloff(FalloffMagnitude,0.8f, 1.f, 0.f, SphereRadius, GetActorLocation(), EFieldFalloffType::Field_FallOff_None);	
	RadialVector->SetRadialVector(VectorMagnitude, GetActorLocation());
	
	if (GetFieldSystemComponent())
	{
		if (RadialFalloff)
		{
			GetFieldSystemComponent()->ApplyPhysicsField(true, Field_ExternalClusterStrain, nullptr, RadialFalloff);
		}

		if (RadialVector)
		{
			GetFieldSystemComponent()->ApplyPhysicsField(true, Field_LinearForce, MetaDataFilter, RadialVector);
		}
	}
}
