// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupStationMinigame.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECTCHENCHEN_API UPickupStationMinigame : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Minigame")
	int32 StopAndEvaluate();
	
	UFUNCTION(BlueprintPure, Category = "Minigame")
	float GetPointerAlpha() const { return PointerAlpha;}
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Minigame")
	void OnZoneLanded(int32 BlocksAwarded);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minigame Tuning", meta = (ClampMin = "0.1"))
	float PointerSpeed = 1.4f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minigame Tuning", meta = (ClampMin = "0.01", ClampMax = "0.5"))
	float GreenHalfWidth = 0.08f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minigame Tuning", meta = (ClampMin = "0.05", ClampMax = "0.5"))
	float LightGreenHalfWidth = 0.25f;
private:
	float PointerAlpha = 0.0f;
	float Direction = 1.0f;
	bool IsMoving = true;
};

