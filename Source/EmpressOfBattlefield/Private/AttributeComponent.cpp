#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHealth = 100.f;
	MaxHealth = 100.f;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}