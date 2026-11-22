// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Block.h"
#include "InventoryComponent.generated.h"

class UShopItemData;

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot", meta = (ClampMin = "0"))
    int32 Capacity = 20;
    
    UPROPERTY(BlueprintReadOnly, Category = "Slot")
    int32 Count = 0;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquippedSlotChanged, EInventorySlot, NewSlot, FBlockSlotData, SlotData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPurchasedItemAdded, UShopItemData*, Item, int32, NewQuantity);

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
	
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddBlockByType(EBlockType BrokenType);
    
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool ConsumeEquipped();
    
	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool CanPlaceEquipped() const;
    
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetCount(EInventorySlot Slot) const;

	// For powerups
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop Inventory")
	TMap<TObjectPtr<UShopItemData>, int32> PurchasedItems;

	UPROPERTY(BlueprintAssignable, Category = "Shop Inventory")
	FOnPurchasedItemAdded OnPurchasedItemAdded;

	UFUNCTION(BlueprintCallable, Category = "Shop Inventory")
	void AddPurchasedItem(UShopItemData* Item, int32 Quantity = 1);

	UFUNCTION(BlueprintPure, Category = "Shop Inventory") 
	int32 GetPurchasedItemQuantity(UShopItemData* Item) const;

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
