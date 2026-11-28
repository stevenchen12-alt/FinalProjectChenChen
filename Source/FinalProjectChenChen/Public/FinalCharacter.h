// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "InventoryComponent.h"
#include "Block.h"
#include "EnhancedInputSubsystems.h" 
#include "InputActionValue.h"
#include "PreviewBlock.h"
#include "PlayerBuildPad.h"
#include "FinalCharacter.generated.h"

class UAnimBlueprint;
class UInputMappingContext;
class UInputAction;
class UInputComponent;

UCLASS()
class FINALPROJECTCHENCHEN_API AFinalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFinalCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> FirstPersonContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Building")
	TObjectPtr<UInputAction> PlaceBlockAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Building")
	TObjectPtr<UInputAction> BreakBlockAction;

	UPROPERTY()
	TObjectPtr<APlayerBuildPad> BuildPad;
	
	UPROPERTY(Transient)
	TObjectPtr<class APickupStation> ActivePickupStation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Inventory")
	TObjectPtr<UInputAction> ScrollSlotAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Inventory")
	TObjectPtr<UInputAction> SelectSlot1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Inventory")
	TObjectPtr<UInputAction> SelectSlot2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Inventory")
	TObjectPtr<UInputAction> SelectSlot3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Inventory")
	TObjectPtr<UInputAction> SelectSlot4Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Inventory")
	TObjectPtr<UInputAction> SelectSlot5Action;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Inventory")
	TObjectPtr<UInputAction> SelectSlot6Action;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Inventory")
	TObjectPtr<UInputAction> SelectSlot7Action;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Inventory")
	TObjectPtr<UInputAction> SelectSlot8Action;

	void OnScrollSlot(const FInputActionValue& Value);
	void SelectSlot1() { if (InventoryComponent) InventoryComponent->EquipSlot(EInventorySlot::BreakTool); }
	void SelectSlot2() { if (InventoryComponent) InventoryComponent->EquipSlot(EInventorySlot::BlockSlot1); }
	void SelectSlot3() { if (InventoryComponent) InventoryComponent->EquipSlot(EInventorySlot::BlockSlot2); }
	void SelectSlot4() { if (InventoryComponent) InventoryComponent->EquipSlot(EInventorySlot::BlockSlot3); }
	void SelectSlot5() { if (InventoryComponent) InventoryComponent->EquipSlot(EInventorySlot::BlockSlot4); }
	void SelectSlot6() { if (InventoryComponent) InventoryComponent->EquipSlot(EInventorySlot::BlockSlot5); }
	void SelectSlot7() { if (InventoryComponent) InventoryComponent->EquipSlot(EInventorySlot::BlockSlot6); }
	void SelectSlot8() { if (InventoryComponent) InventoryComponent->EquipSlot(EInventorySlot::BlockSlot7); }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
	void PlaceBlock(const FInputActionValue& Value);
	
	UFUNCTION()
	void BreakBlock(const FInputActionValue& Value);
	
	bool IsInConstructionArea(const FVector& WorldLocation, AActor* HitActor, UPrimitiveComponent* HitComponent) const;
	
	bool GetTargetPlacementLocation(FVector& OutSpawnLocation) const;
	
	void SetActivePickupStation(class APickupStation* NewStation) { ActivePickupStation = NewStation; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Materials")
	TArray<TObjectPtr<UMaterialInterface>> AvailableMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Materials")
	TObjectPtr<UMaterialInterface> CurrentMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSubclassOf<ABlock> BlockClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSubclassOf<APreviewBlock> PreviewBlockClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float GridSize = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float MaxReachDistance = 500.0f;
	
	UPROPERTY()
	APreviewBlock* PreviewBlock;
	
	
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;
	
	UPROPERTY(EditAnywhere, Category = Camera)
	FVector FirstPersonCameraOffset = FVector(2.8f, 5.9f, 0.0f);
	
	UPROPERTY(EditAnywhere, Category = Camera)
	float FirstPersonFieldOfView = 70.0f;
	
	UPROPERTY(EditAnywhere, Category = Camera)
	float FirstPersonScale = 0.6f;
	
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> FirstPersonMeshComponent;
	
	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<UAnimBlueprint> FirstPersonDefaultAnim;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float GravitySpeed = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpZVelocity = 420.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float AirControl = 0.2f;
	
	
	
};
