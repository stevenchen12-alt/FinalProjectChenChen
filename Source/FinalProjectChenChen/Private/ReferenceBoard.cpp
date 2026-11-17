// ReferenceBoard.cpp


#include "ReferenceBoard.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBuildPad.h"

// Sets default values
AReferenceBoard::AReferenceBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BoardFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardFrameMesh"));
	BoardFrameMesh->SetupAttachment(RootComponent);

	ScreenWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ScreenWidgetComponent"));
	ScreenWidgetComponent->SetupAttachment(RootComponent);
	ScreenWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	ScreenWidgetComponent->SetDrawSize(FVector2D(512.0f, 512.0f));
}

// Called when the game starts or when spawned
void AReferenceBoard::BeginPlay()
{
	Super::BeginPlay();

	BuildPad = Cast<APlayerBuildPad>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerBuildPad::StaticClass()));

	UpdateDisplayedImage();
}

void AReferenceBoard::UpdateDisplayedImage() {
	if (!BuildPad) {
		BuildPad = Cast<APlayerBuildPad>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerBuildPad::StaticClass()));
	}

	if (!BuildPad || !ScreenWidgetComponent) {
		return;
	}

	if (UUserWidget* Widget = ScreenWidgetComponent->GetWidget()) {
		// Finds an image widget named ReferenceImage inside the assigned widget blueprint
		if (UImage* ImageWidget = Cast<UImage>(Widget->GetWidgetFromName(TEXT("ReferenceImage")))) {
			if (UTexture2D* ReferenceTexture = BuildPad->GetReferenceImage()) {
				ImageWidget->SetBrushFromTexture(ReferenceTexture, true);
			}
		}
	}
}