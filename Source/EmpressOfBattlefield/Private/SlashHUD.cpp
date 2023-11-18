// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashHUD.h"
#include "SlashOverlay.h"

void ASlashHUD::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<APlayerController> Controller = GetWorld()->GetFirstPlayerController();
	if (SlashOverlayWidget && Controller)
	{
		SlashOverlay = CreateWidget<USlashOverlay>(Controller, SlashOverlayWidget);

		if (SlashOverlay)
		{
			SlashOverlay->AddToViewport();
		}
	}
}
