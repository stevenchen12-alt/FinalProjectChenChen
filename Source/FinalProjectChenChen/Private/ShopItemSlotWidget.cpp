// ShopItemSlotWidget.cpp

#include "ShopItemSlotWidget.h"
#include "ShopItemData.h"
#include "InventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/Engine.h"

void UShopItemSlotWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (BuyButton) {
		BuyButton->OnClicked.RemoveDynamic(this, &UShopItemSlotWidget::OnBuyButtonClicked);
		BuyButton->OnClicked.AddDynamic(this, &UShopItemSlotWidget::OnBuyButtonClicked);
	}
}

void UShopItemSlotWidget::SetupSlot(UShopComponent* ShopComp, UInventoryComponent* InventoryComp, int32 ItemIndex, const FShopItem& Item) {
	ShopComponent = ShopComp;
	InventoryComponent = InventoryComp;
	ListingIndex = ItemIndex;

	if (!Item.Item) {
		return;
	}

	FString DebugMsg = FString::Printf(
		TEXT("ItemIcon valid: %d | DataAsset Icon valid: %d"),
		ItemIcon != nullptr,
		(Item.Item && Item.Item->Icon != nullptr)
	);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, DebugMsg);

	if (ItemIcon && Item.Item->Icon) {
		ItemIcon->SetBrushFromTexture(Item.Item->Icon, false);
	}

	if (ItemNameText) {
		ItemNameText->SetText(Item.Item->DisplayName);
	}

	if (PriceText) {
		PriceText->SetText(FText::Format(FText::FromString(TEXT("Price: {0}")), FText::AsNumber(Item.GetPrice())));
	}

	if (StockText) {
		if (Item.Stock == -1) {
			StockText->SetText(FText::FromString(TEXT("Stock: Unlimited")));
		}

		else if (Item.Stock == -1) {
			StockText->SetText(FText::FromString(TEXT("Sold out")));
		}

		else {
			StockText->SetText(FText::Format(FText::FromString(TEXT("Stock: {0}")), FText::AsNumber(Item.Stock)));
		}
	}

	// Enable or disable the buy button depending on available money and stock
	if (BuyButton && ShopComponent && InventoryComponent) {
		bool bCanBuy = ShopComponent->CanPurchaseItem(InventoryComponent, ListingIndex, 1);
		BuyButton-> SetIsEnabled(bCanBuy);
	}
}

void UShopItemSlotWidget::OnBuyButtonClicked() {
	if (ShopComponent && InventoryComponent) {
		ShopComponent->TryPurchaseItem(InventoryComponent, ListingIndex, 1);
	}
}