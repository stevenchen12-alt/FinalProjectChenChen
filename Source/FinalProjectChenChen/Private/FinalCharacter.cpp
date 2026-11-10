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
