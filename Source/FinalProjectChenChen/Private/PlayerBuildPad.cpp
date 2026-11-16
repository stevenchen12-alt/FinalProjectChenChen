// PlayerBuildPad.cpp


#include "PlayerBuildPad.h"

// Sets default values
APlayerBuildPad::APlayerBuildPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

FIntVector APlayerBuildPad::WorldToGrid(const FVector& WorldLocation) const {
	float GridSize = 100.0f;

	if (ActiveTemplate) {
		GridSize = ActiveTemplate->GridSize;
	}

	FVector LocalPosition = GetActorTransform().InverseTransformPosition(WorldLocation);

	return FIntVector(FMath::RoundToInt(LocalPosition.X / GridSize), FMath::RoundToInt(LocalPosition.Y / GridSize), FMath::RoundToInt(LocalPosition.Z / GridSize));
}

void APlayerBuildPad::NotifyBlockPlaced(ABlock* Block) {
	if (!Block) {
		return;
	}

	// Get the block's position and material and add it to player layout
	FIntVector Position = WorldToGrid(Block->GetActorLocation());
	UMaterialInterface* Material = Block->GetBlockMaterial();

	PlayerLayout.Add(Position, Material);
}

void APlayerBuildPad::NotifyBlockRemoved(ABlock* Block) {
	if (!Block) {
		return;
	}

	// Get the block's position and use it to remove it from player layout
	FIntVector Position = WorldToGrid(Block->GetActorLocation());

	PlayerLayout.Remove(Position);
}

TMap<FIntVector, TObjectPtr<UMaterialInterface>> APlayerBuildPad::GetShiftedLayout(const TMap<FIntVector, TObjectPtr<UMaterialInterface>>& OriginalLayout) {
	TMap<FIntVector, TObjectPtr<UMaterialInterface>> Result;

	if (OriginalLayout.IsEmpty()) {
		return Result;
	}

	FIntVector MinCoordinates(INT32_MAX, INT32_MAX, INT32_MAX);

	for (const TPair<FIntVector, TObjectPtr<UMaterialInterface>>& Entry : OriginalLayout) {
		MinCoordinates.X = FMath::Min(MinCoordinates.X, Entry.Key.X);
		MinCoordinates.Y = FMath::Min(MinCoordinates.Y, Entry.Key.Y);
		MinCoordinates.Z = FMath::Min(MinCoordinates.Z, Entry.Key.Z);
	}

	// Allot space for OriginalLayout.Num() entries
	Result.Reserve(OriginalLayout.Num());

	for (const TPair<FIntVector, TObjectPtr<UMaterialInterface>>& Entry : OriginalLayout) {
		Result.Add(Entry.Key - MinCoordinates, Entry.Value);
	}

	return Result;
}

FBuildEvaluationResult APlayerBuildPad::EvaluateBuild() const {
	FBuildEvaluationResult Result;

	if (!ActiveTemplate) {
		return Result;
	}

	TMap<FIntVector, TObjectPtr<UMaterialInterface>>& TargetLayout = ActiveTemplate->Layout;

	Result.TotalTargetBlocks = TargetLayout.Num();

	if (Result.TotalTargetBlocks == 0 || PlayerLayout.IsEmpty()) {
		return Result;
	}

	TMap<FIntVector, TObjectPtr<UMaterialInterface>> ShiftedPlayer = GetShiftedLayout(PlayerLayout);
	TMap<FIntVector, TObjectPtr<UMaterialInterface>> ShiftedTarget = GetShiftedLayout(TargetLayout);

	for (const TPair<FIntVector, TObjectPtr<UMaterialInterface>>& TargetEntry : ShiftedTarget) {
		if (const TObjectPtr<UMaterialInterface>* PlayerMaterial = ShiftedPlayer.Find(TargetEntry.Key)) {
			if (*PlayerMaterial == TargetEntry.Value) {
				Result.NumCorrectBlocks++;
			}
		}
	}

	Result.NumWrongBlocks = Result.TotalTargetBlocks - Result.NumCorrectBlocks;

	int32 NetCorrect = FMath::Max(0, Result.NumCorrectBlocks - Result.NumWrongBlocks);
	Result.Accuracy = (static_cast<float>(NetCorrect) / static_cast<float>(Result.TotalTargetBlocks)) * 100.0f;

	if (Result.NumCorrectBlocks == Result.TotalTargetBlocks) {
		Result.bIsPerfectMatch = true;
	}

	else {
		Result.bIsPerfectMatch = false;
	}

	return Result;
}

void APlayerBuildPad::ResetTrackedLayout() {
	PlayerLayout.Empty();
}

UTexture2D* APlayerBuildPad::GetReferenceImage() const {
	if (ActiveTemplate) {
		return ActiveTemplate->ReferenceImage;
	}

	return nullptr;
}