// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashHUD.generated.h"

class USlashOverlay;
/**
 * 
 */
UCLASS()
class EMPRESSOFBATTLEFIELD_API ASlashHUD : public AHUD
{
	GENERATED_BODY()

public:

	void BeginPlay() override;

	FORCEINLINE TObjectPtr<USlashOverlay> GetSlashOverlay() { return SlashOverlay; };

private:

	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<USlashOverlay> SlashOverlayWidget;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USlashOverlay> SlashOverlay;
};
