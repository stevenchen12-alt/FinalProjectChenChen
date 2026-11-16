// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SubmitButton.generated.h"

class APlayerBuildPad;

UCLASS()
class FINALPROJECTCHENCHEN_API ASubmitButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASubmitButton();

	// Called automatically when the player steps onto the button's existing overlap volume
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Reset button so it can be pressed again
	UFUNCTION(BlueprintCallable, Category = "Submit Button")
	void ResetButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY()
	TObjectPtr<APlayerBuildPad> BuildPad;

	// Prevents duplicate submissions while player stands on the button
	bool bAlreadyTriggered = false;
};
