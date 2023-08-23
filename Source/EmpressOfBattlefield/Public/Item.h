#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"


class USphereComponent;

UCLASS()
class EMPRESSOFBATTLEFIELD_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	virtual void Tick(float DeltaTime) override;
	void SinusoidalMovement();
	
	UFUNCTION()
	virtual void OverlappingBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OverlappingEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mesh")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mesh")
	USphereComponent* SphereCollider;

	UPROPERTY()
	float RunningTime;

	UPROPERTY()
	float SinResult;

	UPROPERTY(EditDefaultsOnly, Category="Offset")
	float OffsetMultiplier;

	UPROPERTY(EditDefaultsOnly, Category="Offset")
	float OffsetSpeed;

	UPROPERTY(VisibleInstanceOnly, Category="Equip")
	bool IsItemTaken;	
};

