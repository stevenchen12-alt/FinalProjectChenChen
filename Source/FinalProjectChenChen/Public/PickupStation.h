#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.h"
#include "PickupStationMinigame.h"
#include "PickupStation.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UPickupMinigameWidget;
class AFinalCharacter;

UCLASS()
class FINALPROJECTCHENCHEN_API APickupStation : public AActor
{
	GENERATED_BODY()

public:
	APickupStation();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void AttemptStopMinigame();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& Sweep);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CloseMinigameUI();
	void Restock();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnAvailabilityChanged(bool bIsAvailable);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	EBlockType BlockType = EBlockType::RedBrick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TSubclassOf<UPickupStationMinigame> MinigameWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
	float RestockSeconds = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> Trigger;

	UPROPERTY(BlueprintReadOnly, Category = "Pickup")
	bool bAvailable = true;

private:
	UPROPERTY(Transient)
	TObjectPtr<UPickupStationMinigame> ActiveWidget;

	UPROPERTY(Transient)
	TObjectPtr<AFinalCharacter> OverlappingPlayer;

	bool bMinigameResolved = false;
	FTimerHandle RestockTimer;
	FTimerHandle CloseUITimer;
};