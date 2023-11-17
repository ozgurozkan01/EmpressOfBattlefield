// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashOverlay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USlashOverlay::SetHealthBarPercentage(float Percentage)
{
	if (HealthBar == nullptr) { return; }

	HealthBar->SetPercent(Percentage);
}

void USlashOverlay::SetStaminaBarPercentage(float Percentage)
{
	if (StaminaBar == nullptr) { return; }

	StaminaBar->SetPercent(Percentage);
}

void USlashOverlay::SetGoldCount(int32 Gold)
{
	if (GoldText == nullptr) { return; }

	const FString GoldCountString = FString::Printf(TEXT("%d"), Gold);
	const FText GoldCount = FText::FromString(GoldCountString);
	GoldText->SetText(GoldCount);
}

void USlashOverlay::SetSoulsCount(int32 Souls)
{
	if (SoulsText == nullptr) { return; }

	const FString SoulsCountString = FString::Printf(TEXT("%d"), Souls);
	const FText SoulsCount = FText::FromString(SoulsCountString);
	GoldText->SetText(SoulsCount);
}