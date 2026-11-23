// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InventoryComponent.h"
#include "FinalHUD.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECTCHENCHEN_API AFinalHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FinalHUD")
	TSubclassOf<class UUserWidget> FinalHUDWidgetClass;

	UPROPERTY()
	UUserWidget* FinalHUDWidget;
	
	UFUNCTION(BlueprintPure, Category = "HUD Data") 
	int32 GetCurrentScore();
	
	UFUNCTION(BlueprintPure, Category = "HUD Data")
	int32 GetSlotCount(EInventorySlot Slot) const;
	
	UFUNCTION(BlueprintPure, Category = "HUD Data")
	FString GetTimeLeft() const;
	
	UFUNCTION(BlueprintPure, Category = "HUD Data")
	int32 GetMoney() const;
}
;