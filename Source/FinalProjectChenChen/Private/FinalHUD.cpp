// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalHUD.h"
#include "FinalGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AFinalHUD::BeginPlay()
{
	Super::BeginPlay();
	FinalHUDWidget = CreateWidget(GetWorld(), FinalHUDWidgetClass);
	FinalHUDWidget->AddToViewport();
}

int32 AFinalHUD::GetCurrentScore()
{
	if (AFinalGameMode* GameMode = Cast<AFinalGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		return GameMode->GetScore();
	}
	return 0;
}