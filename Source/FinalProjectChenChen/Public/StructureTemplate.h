// StructureTemplate.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Materials/MaterialInterface.h"
#include "Engine/Texture2D.h"
#include "StructureTemplate.generated.h"

UCLASS(BlueprintType)
class FINALPROJECTCHENCHEN_API UStructureTemplate : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Structure")
	FText TemplateName;

	// Reference picture displayed to the player on their UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Structure")
	TObjectPtr<UTexture2D> ReferenceImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Structure")
	float GridSize = 100.0f;

	//Maps local x, y, z coordinates to the block's material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structure")
	TMap<FIntVector, TObjectPtr<UMaterialInterface>> Layout;
};
