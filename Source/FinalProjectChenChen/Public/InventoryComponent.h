// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Block.h"
#include "InventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EInventorySlot : uint8
{
	BreakTool UMETA(DisplayName = "Slot 1: Break Tool"),
	BlockSlot1 UMETA(DisplayName = "Slot 2: Red Brick"),
	BlockSlot2 UMETA(DisplayName = "Slot 3: White Brick"),
	BlockSlot3 UMETA(DisplayName = "Slot 4: Tan Brick"),
	BlockSlot4 UMETA(DisplayName = "Slot 5: Stone Brick"),
	BlockSlot5 UMETA(DisplayName = "Slot 6: Cracked Brick"),
	BlockSlot6 UMETA(DisplayName = "Slot 7: Pebble Brick"),
	BlockSlot7 UMETA(DisplayName = "Slot 8: Dirt")
};

USTRUCT(BlueprintType)
struct FBlockSlotData
{	
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot")
	bool bIsBreakTool = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot")
	EBlockType BlockType = EBlockType::RedBrick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot")
	TObjectPtr<UMaterialInterface> BlockMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot")
	FText DisplayName;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquippedSlotChanged, EInventorySlot, NewSlot, FBlockSlotData, SlotData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALPROJECTCHENCHEN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipSlot(EInventorySlot NewSlot);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CycleSlot(float ScrollDirection);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	EInventorySlot GetEquippedSlot() const { return EquippedSlot; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool IsBreakToolEquipped() const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	FBlockSlotData GetEquippedSlotData() const;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnEquippedSlotChanged OnSlotChanged;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TMap<EInventorySlot, FBlockSlotData> Slots;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	EInventorySlot EquippedSlot = EInventorySlot::BreakTool;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
