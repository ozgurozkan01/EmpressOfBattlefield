// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashHUD.h"
#include "SlashOverlay.h"

void ASlashHUD::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<APlayerController> Controller = GetWorld()->GetFirstPlayerController();
	if (SlashOverlayWidget && Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("1"));
		TObjectPtr<USlashOverlay> SlashOverlay = CreateWidget<USlashOverlay>(Controller, SlashOverlayWidget);

		if (SlashOverlay)
		{
			UE_LOG(LogTemp, Warning, TEXT("2"));
			SlashOverlay->AddToViewport();
		}
	}
}
