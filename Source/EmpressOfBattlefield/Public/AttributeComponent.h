#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMPRESSOFBATTLEFIELD_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool IsAlive() const;

	// Getter
	float GetHealthPercentage();
	// Setter
	void SetCurrentHealth(float Damage);
protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category="Custom Attributes")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category="Custom Attributes")
	float MaxHealth;
	
};
