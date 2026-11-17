// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFinalCharacter::AFinalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FirstPersonCameraComponent != nullptr);
	
	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FirstPersonMeshComponent != nullptr);
	
	FirstPersonMeshComponent->SetupAttachment(GetMesh());
	
	FirstPersonMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMeshComponent->SetCollisionProfileName(FName("NoCollision"));
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
	
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMeshComponent, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FirstPersonCameraOffset, FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = FirstPersonFieldOfView;
	FirstPersonCameraComponent->FirstPersonScale = FirstPersonScale;
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void AFinalCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(GEngine != nullptr);

	FirstPersonMeshComponent->SetAnimInstanceClass(FirstPersonDefaultAnim->GeneratedClass);
	GetMesh()->SetAnimInstanceClass(FirstPersonDefaultAnim->GeneratedClass);
	
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->CastShadow = true;
	GetMesh()->bCastHiddenShadow = true;
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FirstPersonContext, 0);
		}
	}
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->GravityScale = GravitySpeed;
	Movement->JumpZVelocity = JumpZVelocity;
	Movement->AirControl = AirControl;
	
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FirstPersonCameraOffset, FRotator(0.0f, 90.0f, -90.0f));

	BuildPad = Cast<APlayerBuildPad>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerBuildPad::StaticClass()));

	if (!CurrentMaterial && AvailableMaterials.IsValidIndex(0)) {
		CurrentMaterial = AvailableMaterials[0];
	}
}

// Called every frame
void AFinalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!PreviewBlock && GetWorld() && PreviewBlockClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		PreviewBlock = GetWorld()->SpawnActor<APreviewBlock>(PreviewBlockClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (PreviewBlock)
		{
			PreviewBlock->SetActorEnableCollision(false);
		}
	}

	if (PreviewBlock)
	{
		bool bHoldingBreakTool = InventoryComponent && InventoryComponent->IsBreakToolEquipped();
		FVector TargetLocation;
		if (!bHoldingBreakTool && GetTargetPlacementLocation(TargetLocation))
		{
			PreviewBlock->SetActorHiddenInGame(false);
			PreviewBlock->SetActorLocation(TargetLocation);
		}
		else
		{
			PreviewBlock->SetActorHiddenInGame(true);
		}
	}
}

// Called to bind functionality to input
void AFinalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFinalCharacter::Move);
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFinalCharacter::Look);
		
		EnhancedInputComponent->BindAction(PlaceBlockAction, ETriggerEvent::Started, this, &AFinalCharacter::PlaceBlock);
		EnhancedInputComponent->BindAction(BreakBlockAction, ETriggerEvent::Started, this, &AFinalCharacter::BreakBlock);
		
		if (ScrollSlotAction) EnhancedInputComponent->BindAction(ScrollSlotAction, ETriggerEvent::Triggered, this, &AFinalCharacter::OnScrollSlot);
		if (SelectSlot1Action) EnhancedInputComponent->BindAction(SelectSlot1Action, ETriggerEvent::Started, this, &AFinalCharacter::SelectSlot1);
		if (SelectSlot2Action) EnhancedInputComponent->BindAction(SelectSlot2Action, ETriggerEvent::Started, this, &AFinalCharacter::SelectSlot2);
		if (SelectSlot3Action) EnhancedInputComponent->BindAction(SelectSlot3Action, ETriggerEvent::Started, this, &AFinalCharacter::SelectSlot3);
		if (SelectSlot4Action) EnhancedInputComponent->BindAction(SelectSlot4Action, ETriggerEvent::Started, this, &AFinalCharacter::SelectSlot4);
		if (SelectSlot5Action) EnhancedInputComponent->BindAction(SelectSlot5Action, ETriggerEvent::Started, this, &AFinalCharacter::SelectSlot5);
		if (SelectSlot6Action) EnhancedInputComponent->BindAction(SelectSlot6Action, ETriggerEvent::Started, this, &AFinalCharacter::SelectSlot6);
		if (SelectSlot7Action) EnhancedInputComponent->BindAction(SelectSlot7Action, ETriggerEvent::Started, this, &AFinalCharacter::SelectSlot7);
		if (SelectSlot8Action) EnhancedInputComponent->BindAction(SelectSlot8Action, ETriggerEvent::Started, this, &AFinalCharacter::SelectSlot8);
	}
}

void AFinalCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();
	
	if (Controller)
	{
		const FVector Right = GetActorRightVector();
		AddMovementInput(Right, MovementValue.X);
		
		const FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, MovementValue.Y);
	}
}

void AFinalCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

bool AFinalCharacter::GetTargetPlacementLocation(FVector& OutSpawnLocation) const
{
	if (!FirstPersonCameraComponent) return false;

	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector LookVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndLocation = StartLocation + (LookVector * MaxReachDistance);
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	
	if (bHit && HitResult.bBlockingHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsInConstructionArea(HitResult.ImpactPoint, HitActor, HitResult.GetComponent()))
		{
			return false;
		}
		
		if (HitActor && HitActor->ActorHasTag(FName("PlacedBlock")))
		{
			FVector BaseLocation = HitActor->GetActorLocation();
			OutSpawnLocation = BaseLocation + (HitResult.ImpactNormal * GridSize);
		}
		else
		{
			OutSpawnLocation.X = FMath::RoundToFloat(HitResult.ImpactPoint.X / GridSize) * GridSize;
			OutSpawnLocation.Y = FMath::RoundToFloat(HitResult.ImpactPoint.Y / GridSize) * GridSize;
			
			if (HitResult.ImpactNormal.Z > 0.5f)
			{
				OutSpawnLocation.Z = 0.0f;
			}
			else
			{
				OutSpawnLocation.Z = FMath::RoundToFloat((HitResult.ImpactPoint.Z + (HitResult.ImpactNormal.Z * 1.0f)) / GridSize) * GridSize;
			}
		}
		
		OutSpawnLocation.X = FMath::RoundToFloat(OutSpawnLocation.X / GridSize) * GridSize;
		OutSpawnLocation.Y = FMath::RoundToFloat(OutSpawnLocation.Y / GridSize) * GridSize;
		OutSpawnLocation.Z = FMath::RoundToFloat(OutSpawnLocation.Z / GridSize) * GridSize;
		
		if (OutSpawnLocation.Z < 0.0f)
		{
			OutSpawnLocation.Z = 0.0f;
		}
		
		return true;
	}
	
	return false;
}

void AFinalCharacter::PlaceBlock(const FInputActionValue& Value)
{
	if (!BlockClass) return;
	
	if (InventoryComponent->IsBreakToolEquipped())
	{
		return;
	}
	
	FVector SnappedLocation;
	if (GetTargetPlacementLocation(SnappedLocation))
	{
		FVector BlockCenter = SnappedLocation + FVector(0.0f, 0.0f, GridSize * 0.5f);
		FVector BlockExtent = FVector(GridSize * 0.45f);
		
		TArray<FOverlapResult> OverlapResults;
		FCollisionShape BoxShape = FCollisionShape::MakeBox(BlockExtent);
		FCollisionQueryParams OverlapParams;
		OverlapParams.AddIgnoredActor(this);
		
		bool bIsOverlappingPlayer = GetWorld()->OverlapMultiByChannel(
			OverlapResults, BlockCenter, FQuat::Identity, ECC_Pawn, BoxShape, OverlapParams
		);
		
		if (bIsOverlappingPlayer) return;
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(BlockClass, SnappedLocation, FRotator::ZeroRotator, SpawnParams);
		if (NewBlock)
		{
			NewBlock->Tags.Add(FName("PlacedBlock"));

			if (CurrentMaterial) {
				NewBlock->SetBlockMaterial(CurrentMaterial);
			}

			if (BuildPad)
			{
				BuildPad->NotifyBlockPlaced(NewBlock);
			}
			FBlockSlotData SlotData = InventoryComponent->GetEquippedSlotData();
			NewBlock->InitializeBlock(SlotData.BlockType, SlotData.BlockMaterial);
		}
	}
}
void AFinalCharacter::BreakBlock(const FInputActionValue& Value)
{
	if (!FirstPersonCameraComponent) return;
	
	if (!InventoryComponent->IsBreakToolEquipped())
	{
		return;
	}

	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = StartTrace + (ForwardVector * MaxReachDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, QueryParams))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->ActorHasTag(FName("PlacedBlock")))
		{
			if (BuildPad)
			{
				if (ABlock* HitBlock = Cast<ABlock>(HitActor))
				{
					BuildPad->NotifyBlockRemoved(HitBlock);
				}
			}
			HitActor->Destroy();
		}
	}
}

bool AFinalCharacter::IsInConstructionArea(const FVector& WorldLocation, AActor* HitActor, UPrimitiveComponent* HitComponent) const
{
	if (HitActor && HitActor->ActorHasTag(FName("PlacedBlock")))
	{
		return true;
	}
	
	if (HitComponent && HitComponent->ComponentHasTag(FName("DesignatedZone")))
	{
		return true;
	}

	return false;
}

void AFinalCharacter::OnScrollSlot(const FInputActionValue& Value)
{
	if (InventoryComponent)
	{
		InventoryComponent->CycleSlot(Value.Get<float>());
	}
}

