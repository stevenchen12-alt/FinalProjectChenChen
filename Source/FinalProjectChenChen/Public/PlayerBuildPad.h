// PlayerBuildPad.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructureTemplate.h"
#include "Block.h"
#include "PlayerBuildPad.generated.h"

USTRUCT(BlueprintType)
struct FBuildEvaluationResult {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 NumCorrectBlocks = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 NumWrongBlocks = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 TotalTargetBlocks = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	float Accuracy = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	bool bIsPerfectMatch = false;
};


UCLASS()
class FINALPROJECTCHENCHEN_API APlayerBuildPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBuildPad();

	// The template the player is currently trying to copy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validation")
	TObjectPtr<UStructureTemplate> ActiveTemplate;

	// Called after placing a block
	UFUNCTION(BlueprintCallable, Category = "Validation")
	void NotifyBlockPlaced(ABlock* Block);

	// Called after destroying a block
	UFUNCTION(BlueprintCallable, Category = "Validation")
	void NotifyBlockRemoved(ABlock* Block);

	// Called from HUD's done button to get result and pass it to game mode
	UFUNCTION(BlueprintPure, Category = "Validation")
	FBuildEvaluationResult EvaluateBuild() const;

	// Clears tracked map when resetting the level
	UFUNCTION(BlueprintCallable, Category = "Validation")
	void ResetTrackedLayout();

	// Helper for HUD to get current template's reference image
	UFUNCTION(BlueprintPure, Category = "Validation")
	UTexture2D* GetReferenceImage() const;

	// Converts block's world location into local x, y, z coordinates
	UFUNCTION(BlueprintPure, Category = "Validation")
	FIntVector WorldToGrid(const FVector& WorldLocation) const;

private:
	// Tracks the player's current blocks
	UPROPERTY(VisibleAnywhere, Category = "Validation")
	TMap<FIntVector, TObjectPtr<UMaterialInterface>> PlayerLayout;

	// Shifts all coordinates in a layout so its minimum x, y, z corner becomes (0, 0, 0)
	static TMap<FIntVector, TObjectPtr<UMaterialInterface>> GetShiftedLayout(const TMap<FIntVector, TObjectPtr<UMaterialInterface>>& OriginalLayout);
};
