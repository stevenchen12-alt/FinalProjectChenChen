#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.h"
#include "PickupStation.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class FINALPROJECTCHENCHEN_API APickupStation : public AActor
{
	GENERATED_BODY()

public:
	APickupStation();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					   bool bFromSweep, const FHitResult& Sweep);

	void Restock();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	EBlockType BlockType = EBlockType::RedBrick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (ClampMin = "1"))
	int32 AmountPerPickup = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
	float RestockSeconds = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> Trigger;

	UPROPERTY(BlueprintReadOnly, Category = "Pickup")
	bool bAvailable = true;

	FTimerHandle RestockTimer;
};