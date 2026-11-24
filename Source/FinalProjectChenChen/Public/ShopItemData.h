// ShopItemData.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShopItemData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FINALPROJECTCHENCHEN_API UShopItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UStaticMesh> ItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "0"))
	int32 BasePrice = 500;
};
