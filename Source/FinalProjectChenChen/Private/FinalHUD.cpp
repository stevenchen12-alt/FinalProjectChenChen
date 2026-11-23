// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalHUD.h"
#include "FinalGameMode.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "FinalCharacter.h"
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

int32 AFinalHUD::GetSlotCount(EInventorySlot Slot) const
{
	if (AFinalCharacter* Character = Cast<AFinalCharacter>(GetOwningPawn()))
	{
		if (Character->InventoryComponent)
		{
			return Character->InventoryComponent->GetCount(Slot);
		}
	}
	return 0;
}

FString AFinalHUD::GetTimeLeft() const
{
	if (AFinalGameMode* GameMode = Cast<AFinalGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		float TimeRemaining = GameMode->GetTimeRemaining();
		int32 Minutes = FMath::FloorToInt(TimeRemaining / 60.f);
		int32 Seconds = FMath::Modulo(TimeRemaining, 60.f);
		return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	}
	return TEXT("00:00");
}

int32 AFinalHUD::GetMoney() const
{
	if (AFinalGameMode* GameMode = Cast<AFinalGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		return GameMode->GetMoneyCollected();
	}
	return 0;
}
