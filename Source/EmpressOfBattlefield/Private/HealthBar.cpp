#include "HealthBar.h"

#include "RendererUtils.h"
#include "Components/ProgressBar.h"

void UHealthBar::SetHealthBarColor(float DamageTaken)
{
	if (HealthBar)
	{
		Red = FMath::Clamp(Red + DamageTaken * 0.01, 0.f, 1.f);
		Green = FMath::Clamp(Green - DamageTaken * 0.01, 0.f, 1.f);

		HealthBar->SetFillColorAndOpacity(FLinearColor(Red, Green, Blue, Alpha));
	}
}
