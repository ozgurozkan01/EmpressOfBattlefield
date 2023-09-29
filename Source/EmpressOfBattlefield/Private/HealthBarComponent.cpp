#include "HealthBarComponent.h"
#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (!GetUserWidgetObject())
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Cyan, TEXT("NO USER OBJECT"));
	}
	
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}