#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class EMPRESSOFBATTLEFIELD_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	UStaticMeshComponent* ItemMesh;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};

