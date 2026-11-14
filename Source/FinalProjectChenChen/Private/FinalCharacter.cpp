// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
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
}

// Called every frame
void AFinalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
void AFinalCharacter::PlaceBlock(const FInputActionValue& Value)
{
	if (!BlockClass || !FirstPersonCameraComponent) return;

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
		if (!IsInConstructionArea(HitResult.ImpactPoint, HitActor))
		{
			return;
		}
		
		FVector SnappedLocation;
		
		if (HitActor && HitActor->ActorHasTag(FName("PlacedBlock")))
		{
			FVector BaseLocation = HitActor->GetActorLocation();
			SnappedLocation = BaseLocation + (HitResult.ImpactNormal * GridSize);
		}
		else
		{
			SnappedLocation.X = FMath::RoundToFloat(HitResult.ImpactPoint.X / GridSize) * GridSize;
			SnappedLocation.Y = FMath::RoundToFloat(HitResult.ImpactPoint.Y / GridSize) * GridSize;
			
			if (HitResult.ImpactNormal.Z > 0.5f)
			{
				SnappedLocation.Z = 0.0f;
			}
			else
			{
				SnappedLocation.Z = FMath::RoundToFloat((HitResult.ImpactPoint.Z + (HitResult.ImpactNormal.Z * 1.0f)) / GridSize) * GridSize;
			}
		}
		
		SnappedLocation.X = FMath::RoundToFloat(SnappedLocation.X / GridSize) * GridSize;
		SnappedLocation.Y = FMath::RoundToFloat(SnappedLocation.Y / GridSize) * GridSize;
		SnappedLocation.Z = FMath::RoundToFloat(SnappedLocation.Z / GridSize) * GridSize;
		
		if (SnappedLocation.Z < 0.0f)
		{
			SnappedLocation.Z = 0.0f;
		}
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(BlockClass, SnappedLocation, FRotator::ZeroRotator, SpawnParams);
		if (NewBlock)
		{
			NewBlock->Tags.Add(FName("PlacedBlock"));
		}
	}
}
void AFinalCharacter::BreakBlock(const FInputActionValue& Value)
{
	if (!FirstPersonCameraComponent) return;

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
			HitActor->Destroy();
		}
	}
}

bool AFinalCharacter::IsInConstructionArea(const FVector& WorldLocation, AActor* HitActor) const
{
	if (HitActor && HitActor->ActorHasTag(FName("PlacedBlock")))
	{
		return true;
	}
	
	if (HitActor && HitActor->ActorHasTag(FName("DesignatedZone")))
	{
		return true;
	}

	return false;
}