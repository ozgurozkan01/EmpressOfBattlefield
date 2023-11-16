#include "AttributeComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UAttributeComponent::IsAlive() const
{
	return CurrentHealth > 0;
}

float UAttributeComponent::GetHealthPercentage()
{
	return CurrentHealth / MaxHealth;
}

void UAttributeComponent::SetCurrentHealth(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
}

void UAttributeComponent::ActivateRagdoll()
{
	TObjectPtr<ACharacter> Owner = Cast<ACharacter>(GetOwner());

	if (Owner)
	{
		if (Owner->GetController())
		{
			Owner->GetController()->SetIgnoreMoveInput(true);
		}

		if (Owner->GetMesh())
		{
			Owner->GetMesh()->SetCollisionProfileName(FName("Ragdoll"), true);
			Owner->GetMesh()->SetAllBodiesSimulatePhysics(true);
		}
	}
}
