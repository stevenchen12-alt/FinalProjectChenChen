// ShopComponent.cpp


#include "ShopComponent.h"
#include "ShopItemData.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FinalGameMode.h"

// Sets default values for this component's properties
UShopComponent::UShopComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

int32 FShopItem::GetPrice() const {
	if (PriceOverride >= 0) {
		return PriceOverride;
	}

	if (Item) {
		return Item->BasePrice;
	}

	return 0;
}

AFinalGameMode* UShopComponent::GetFinalGameMode() const {
	return Cast<AFinalGameMode>(UGameplayStatics::GetGameMode(this));
}

bool UShopComponent::CanPurchaseItem(const UInventoryComponent* BuyerInventory, int32 ListingIndex, int32 Quantity) const {
	if (!BuyerInventory || Quantity <= 0 || !Catalog.IsValidIndex(ListingIndex)) {
		return false;
	}

	const FShopItem& Item = Catalog[ListingIndex];

	if (!Item.Item) {
		return false;
	}

	if (Item.Stock != -1 && Item.Stock < Quantity) {
		return false;
	}

	const AFinalGameMode* GameMode = GetFinalGameMode();

	if (!GameMode) {
		return false;
	}

	int32 TotalCost = Item.GetPrice() * Quantity;
	
	return GameMode->GetMoneyCollected() >= TotalCost;
}

bool UShopComponent::TryPurchaseItem(UInventoryComponent* BuyerInventory, int32 ListingIndex, int32 Quantity) {
	if (!CanPurchaseItem(BuyerInventory, ListingIndex, Quantity)) {
		return false;
	}

	AFinalGameMode* GameMode = GetFinalGameMode();

	if (!GameMode) {
		return false;
	}

	FShopItem& Item = Catalog[ListingIndex];

	int32 TotalCost = Item.GetPrice() * Quantity;

	if (GameMode->SpendMoney(TotalCost)) {
		BuyerInventory->AddPurchasedItem(Item.Item, Quantity);

		if (Item.Stock != -1) {
			Item.Stock -= Quantity;
		}

		OnShopCatalogUpdated.Broadcast();

		return true;
	}

	return false;
}