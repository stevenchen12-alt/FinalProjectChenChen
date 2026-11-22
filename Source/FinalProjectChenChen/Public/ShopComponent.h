// ShopComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShopComponent.generated.h"

class UShopItemData;
class UInventoryComponent;
class AFinalGameMode;

USTRUCT(BlueprintType)
struct FShopItem {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TObjectPtr<UShopItemData> Item = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int32 PriceOverride = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop", meta = (ClampMin = "-1"))
	int32 Stock = -1; // -1 stock means infinite stock

	int32 GetPrice() const;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopCatalogUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALPROJECTCHENCHEN_API UShopComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShopComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop")
	TArray<FShopItem> Catalog;

	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnShopCatalogUpdated OnShopCatalogUpdated;

	UFUNCTION(BlueprintPure, Category = "Shop")
	bool CanPurchaseItem(const UInventoryComponent* BuyerInventory, int32 ListingIndex, int32 Quantity = 1) const;

	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool TryPurchaseItem(UInventoryComponent* BuyerInventory, int32 ListingIndex, int32 Quantity = 1);

private:
	AFinalGameMode* GetFinalGameMode() const;
};
