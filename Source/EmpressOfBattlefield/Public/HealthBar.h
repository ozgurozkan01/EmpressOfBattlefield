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

public:
	FORCEINLINE UProgressBar* GetHealthBarWidget() { return HealthBar; };
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
};
