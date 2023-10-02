// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;
class FCanvasView;
class UCanvas;

UCLASS()
class EMPRESSOFBATTLEFIELD_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

	void NativeConstruct() override;
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

private:

	UPROPERTY(EditDefaultsOnly, Category="Color Values")
	float Red = 0.f;
	UPROPERTY(EditDefaultsOnly, Category="Color Values")
	float Green = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="Color Values")
	float Blue = 0.f;
	UPROPERTY(EditDefaultsOnly, Category="Color Values")
	float Alpha = 1.f;

public:

	// Getter
	FORCEINLINE UProgressBar* GetHealthProgressBar() { return HealthBar; };

	// Setter
	void SetHealthBarColor(float DamageTaken);
};
