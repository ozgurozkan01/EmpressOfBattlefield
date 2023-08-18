#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"


class UCapsuleComponent;

UCLASS()
class EMPRESSOFBATTLEFIELD_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	UCapsuleComponent* CapsuleCollider;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};

