// Fill out your copyright notice in the Description page of Project Settings.


#include "SubmitButton.h"
#include "FinalGameMode.h"
#include "FinalCharacter.h"
#include "PlayerBuildPad.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASubmitButton::ASubmitButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASubmitButton::BeginPlay()
{
	Super::BeginPlay();
	
	BuildPad = Cast<APlayerBuildPad>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerBuildPad::StaticClass()));
}

void ASubmitButton::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bAlreadyTriggered || !OtherActor) {
		return;
	}

	// Only trigger if it's a player character that steps onto the button
	if (OtherActor->IsA<AFinalCharacter>()) {
		bAlreadyTriggered = true;


		if (!BuildPad) {
			BuildPad = Cast<APlayerBuildPad>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerBuildPad::StaticClass()));
		}

		if (BuildPad) {
			FBuildEvaluationResult Result = BuildPad->EvaluateBuild();

			if (AFinalGameMode* GameMode = Cast<AFinalGameMode>(UGameplayStatics::GetGameMode(GetWorld()))) {
				GameMode->SubmitBuildResult(Result);
			}
		}

		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ASubmitButton: No PlayerBuildPad found in level!"));
		}
	}
}

void ASubmitButton::ResetButton() {
	bAlreadyTriggered = false;
}