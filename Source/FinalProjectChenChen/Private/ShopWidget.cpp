// ShopWidget.cpp

#include "ShopWidget.h"
#include "ShopComponent.h"
#include "ShopItemSlotWidget.h"
#include "InventoryComponent.h"
#include "FinalGameMode.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WrapBoxSlot.h"
#include "Kismet/GameplayStatics.h"

void UShopWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (CloseButton) {
		CloseButton->OnClicked.RemoveDynamic(this, &UShopWidget::OnCloseButtonClicked);
		CloseButton->OnClicked.AddDynamic(this, &UShopWidget::OnCloseButtonClicked);
	}
}

void UShopWidget::NativeDestruct() {
	if (ShopComponent) {
		ShopComponent->OnShopCatalogUpdated.RemoveDynamic(this, &UShopWidget::RefreshCatalog);
	}

	if (AFinalGameMode* GameMode = Cast<AFinalGameMode>(UGameplayStatics::GetGameMode(this))) {
		GameMode->OnMoneyChanged.RemoveDynamic(this, &UShopWidget::UpdateMoneyDisplay);
	}

	Super::NativeDestruct();
}

void UShopWidget::InitializeShop(UShopComponent* ShopComp, UInventoryComponent* InventoryComp) {
	ShopComponent = ShopComp;
	InventoryComponent = InventoryComp;

	if (ShopComponent) {
		ShopComponent->OnShopCatalogUpdated.RemoveDynamic(this, &UShopWidget::RefreshCatalog);
		ShopComponent->OnShopCatalogUpdated.AddDynamic(this, &UShopWidget::RefreshCatalog);
	}

	if (AFinalGameMode* GameMode = Cast<AFinalGameMode>(UGameplayStatics::GetGameMode(this))) {
		GameMode->OnMoneyChanged.RemoveDynamic(this, &UShopWidget::UpdateMoneyDisplay);
		GameMode->OnMoneyChanged.AddDynamic(this, &UShopWidget::UpdateMoneyDisplay);

		UpdateMoneyDisplay(GameMode->GetMoneyCollected());
	}

	RefreshCatalog();
}

void UShopWidget::RefreshCatalog() {
	if (!ItemsContainer || !ShopComponent || !ItemSlotClass) {
		return;
	}

	ItemsContainer->ClearChildren();

	for (int32 i = 0; i < ShopComponent->Catalog.Num(); i++) {
		if (UShopItemSlotWidget* SlotWidget = CreateWidget<UShopItemSlotWidget>(this, ItemSlotClass)) {
			
			if (UWrapBoxSlot* BoxSlot = Cast<UWrapBoxSlot>(ItemsContainer->AddChild(SlotWidget))) {
				BoxSlot->SetHorizontalAlignment(HAlign_Center);
				BoxSlot->SetVerticalAlignment(VAlign_Center);
				//BoxSlot->SetPadding(FMargin(0.0f, 10.0f));
			}

			SlotWidget->SetupSlot(ShopComponent, InventoryComponent, i, ShopComponent->Catalog[i]);
		}
	}
}

void UShopWidget::UpdateMoneyDisplay(int32 NewAmount) {
	if (MoneyCollectedText) {
		MoneyCollectedText->SetText(FText::Format(FText::FromString(TEXT("Money Collected: {0}")), FText::AsNumber(NewAmount)));
	}
}

void UShopWidget::OnCloseButtonClicked() {
	if (AFinalGameMode* GameMode = Cast<AFinalGameMode>(UGameplayStatics::GetGameMode(this))) {
		GameMode->ToggleShop();
	}

	else {
		CloseShop();
	}
}

void UShopWidget::CloseShop() {
	if (APlayerController* PC = GetOwningPlayer()) {
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}

	RemoveFromParent();
}