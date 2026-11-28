// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupStationMinigame.h"

void UPickupStationMinigame::NativeConstruct()
{
	Super::NativeConstruct();
	PointerAlpha = 0.0f;
	Direction = 1.0f;
	IsMoving = true;
}

void UPickupStationMinigame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!IsMoving)
	{
		return;
	}
	PointerAlpha += Direction * PointerSpeed * InDeltaTime;
	
	if (PointerAlpha >= 1.0f)
	{
		PointerAlpha = 1.0f;
		Direction = -1.0f;
	} else if (PointerAlpha <= 0.0f)
	{
		PointerAlpha = 0.0f;
		Direction = 1.0f;
	}
}

int32 UPickupStationMinigame::StopAndEvaluate()
{
	if (!IsMoving)
	{
		return 0;
	}
	IsMoving = false;
	
	const float DistanceFromCenter = FMath::Abs(PointerAlpha - 0.5f);
	int32 BlocksRewarded = 0;
	if (DistanceFromCenter <= GreenHalfWidth)
	{
		BlocksRewarded = 3;
	} else if (DistanceFromCenter <= LightGreenHalfWidth)
	{
		BlocksRewarded = 1;
	} else
	{
		BlocksRewarded = 0;
	}
	
	OnZoneLanded(BlocksRewarded);
	return BlocksRewarded;
}

