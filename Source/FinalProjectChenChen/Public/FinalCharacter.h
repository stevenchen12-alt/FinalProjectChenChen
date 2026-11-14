// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "Block.h"
#include "EnhancedInputSubsystems.h" 
#include "InputActionValue.h"
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
	
	bool IsInConstructionArea(const FVector& WorldLocation, AActor* HitActor) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSubclassOf<ABlock> BlockClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float GridSize = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float MaxReachDistance = 500.0f;
	
	
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
