// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

UENUM(BlueprintType)
enum class EBlockType : uint8
{
	None UMETA(DisplayName = "None"),
	RedBrick UMETA(DisplayName = "Red Brick"),
	WhiteBrick UMETA(DisplayName = "White Brick"),
	TanBrick UMETA(DisplayName = "Tan Brick"),
	StoneBrick UMETA(DisplayName = "Stone Brick"),
	CrackedBrick UMETA(DisplayName = "Cracked Brick"),
	PebbleBrick UMETA(DisplayName = "Pebble Brick"),
	Dirt UMETA(DisplayName = "Dirt")
};

UCLASS()
class FINALPROJECTCHENCHEN_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock();

	UFUNCTION(BlueprintPure, Category = "Block")
	UMaterialInterface* GetBlockMaterial() const;
	
	UFUNCTION(BlueprintCallable, Category = "Block")
	void SetBlockMaterial(UMaterialInterface* NewMaterial);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block Data")
	EBlockType BlockType = EBlockType::RedBrick;
	
	UFUNCTION(BlueprintCallable, Category = "Block Data")
	void InitializeBlock(EBlockType NewType, UMaterialInterface* NewMaterial);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
