// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashOverlay.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class EMPRESSOFBATTLEFIELD_API USlashOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealthBarPercentage(float Percentage);
	void SetStaminaBarPercentage(float Percentage);
	void SetGoldCount(int32 Gold);
	void SetSoulsCount(int32 Souls);
	
	
private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> GoldText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SoulsText;
};
