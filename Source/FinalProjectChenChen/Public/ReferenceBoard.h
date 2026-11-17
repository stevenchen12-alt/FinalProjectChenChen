// ReferenceBoard.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReferenceBoard.generated.h"

class UWidgetComponent;
class UStaticMeshCompoennt;
class APlayerBuildPad;

UCLASS()
class FINALPROJECTCHENCHEN_API AReferenceBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReferenceBoard();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BoardFrameMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> ScreenWidgetComponent;

	UFUNCTION(BlueprintCallable, Category = "Reference Board")
	void UpdateDisplayedImage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<APlayerBuildPad> BuildPad;
};
