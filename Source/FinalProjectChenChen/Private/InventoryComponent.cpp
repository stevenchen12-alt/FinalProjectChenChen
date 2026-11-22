// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	for (TPair<EInventorySlot, FBlockSlotData>& Pair : Slots)
	{
		if (!Pair.Value.bIsBreakTool)
		{
			Pair.Value.Count = Pair.Value.Capacity;
		}
	}
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::EquipSlot(EInventorySlot NewSlot)
{
	if (EquippedSlot != NewSlot && Slots.Contains(NewSlot))
	{
		EquippedSlot = NewSlot;
		OnSlotChanged.Broadcast(EquippedSlot, Slots[EquippedSlot]);
	}
}

void UInventoryComponent::CycleSlot(float ScrollDirection)
{
	if (Slots.Num() == 0) return;

	TArray<EInventorySlot> Keys;
	Slots.GetKeys(Keys);

	int32 CurrentIndex = Keys.IndexOfByKey(EquippedSlot);
	if (CurrentIndex == INDEX_NONE)
	{
		CurrentIndex = 0;
	}

	if (ScrollDirection > 0.0f)
	{
		CurrentIndex = (CurrentIndex + 1) % Keys.Num();
	}
	else if (ScrollDirection < 0.0f)
	{
		CurrentIndex = (CurrentIndex - 1 + Keys.Num()) % Keys.Num();
	}

	EquipSlot(Keys[CurrentIndex]);
}

bool UInventoryComponent::IsBreakToolEquipped() const
{
	if (const FBlockSlotData* Data = Slots.Find(EquippedSlot))
	{
		return Data->bIsBreakTool;
	}
	return false;
}

FBlockSlotData UInventoryComponent::GetEquippedSlotData() const
{
	if (const FBlockSlotData* Data = Slots.Find(EquippedSlot))
	{
		return *Data;
	}
	return FBlockSlotData();
}

bool UInventoryComponent::AddBlockByType(EBlockType BrokenType)
{
	if (BrokenType == EBlockType::None)
	{
		return false;
	}

	for (TPair<EInventorySlot, FBlockSlotData>& Pair : Slots)
	{
		FBlockSlotData& SlotData = Pair.Value;
		if (!SlotData.bIsBreakTool && SlotData.BlockType == BrokenType)
		{
			if (SlotData.Count >= SlotData.Capacity)
			{
				return false;
			}

			SlotData.Count++;
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::ConsumeEquipped()
{
	FBlockSlotData* SlotData = Slots.Find(EquippedSlot);
	if (!SlotData || SlotData->bIsBreakTool || SlotData->Count <= 0)
	{
		return false;
	}

	SlotData->Count--;
	OnSlotChanged.Broadcast(EquippedSlot, *SlotData);
	return true;
}

bool UInventoryComponent::CanPlaceEquipped() const
{
	const FBlockSlotData* SlotData = Slots.Find(EquippedSlot);
	return SlotData && !SlotData->bIsBreakTool && SlotData->Count > 0;
}

int32 UInventoryComponent::GetCount(EInventorySlot Slot) const
{
	const FBlockSlotData* SlotData = Slots.Find(Slot);
	if (!SlotData)
	{
		return 0;
	}
	return SlotData->Count;
}