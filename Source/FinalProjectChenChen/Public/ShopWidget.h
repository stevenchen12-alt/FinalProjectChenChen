// ShopWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

class UPanelWidget;
class UTextBlock;
class UButton;
class UShopComponent;
class UInventoryComponent;
class UShopItemSlotWidget;

UCLASS()
class FINALPROJECTCHENCHEN_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Shop UI")
	void InitializeShop(UShopComponent* ShopComp, UInventoryComponent* InventoryComp);

	UFUNCTION(BlueprintCallable, Category = "Shop UI")
	void CloseShop();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Shop UI")
	TSubclassOf<UShopItemSlotWidget> ItemSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ItemsContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MoneyCollectedText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

private:
	UFUNCTION()
	void OnCloseButtonClicked();

	UFUNCTION()
	void RefreshCatalog();

	UFUNCTION()
	void UpdateMoneyDisplay(int32 NewAmount);

	UPROPERTY()
	TObjectPtr<UShopComponent> ShopComponent;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
