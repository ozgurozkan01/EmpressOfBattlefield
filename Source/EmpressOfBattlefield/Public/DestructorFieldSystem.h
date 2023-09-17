// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "DestructorFieldSystem.generated.h"

class UFieldSystemMetaDataFilter;
class URadialVector;
class URadialFalloff;
/**
 * 
 */
UCLASS()
class EMPRESSOFBATTLEFIELD_API ADestructorFieldSystem : public AFieldSystemActor
{
	GENERATED_BODY()

public:
	ADestructorFieldSystem();

protected:

	virtual void BeginPlay() override;
private:

	UPROPERTY(VisibleAnywhere)
	URadialFalloff* RadialFalloff;

	UPROPERTY(VisibleAnywhere)
	URadialVector* RadialVector;

	UPROPERTY(VisibleAnywhere)
	UFieldSystemMetaDataFilter* MetaDataFilter;
	
	UPROPERTY(EditAnywhere)
	float FalloffMagnitude;

	UPROPERTY(EditAnywhere)
	float VectorMagnitude;
	
	UPROPERTY(EditAnywhere)
	float SphereRadius;
};
