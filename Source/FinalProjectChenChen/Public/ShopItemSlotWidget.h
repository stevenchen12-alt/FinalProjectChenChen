// ShopItemSlotWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopComponent.h"
#include "ShopItemSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UInventoryComponent;

UCLASS()
class FINALPROJECTCHENCHEN_API UShopItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetupSlot(UShopComponent* ShopComp, UInventoryComponent* InventoryComp, int32 ItemIndex, const FShopItem& Item);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StockText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BuyButton;

private:
	UFUNCTION()
	void OnBuyButtonClicked();

	UPROPERTY()
	TObjectPtr<UShopComponent> ShopComponent;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	int32 ListingIndex = INDEX_NONE;
};
