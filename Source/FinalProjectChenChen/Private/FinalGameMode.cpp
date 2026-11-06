// FinalGameMode.cpp

#include "FinalGameMode.h"
#include "Blueprint/UserWidget.h"
//#include "ResultWidget.h"
#include "FinalGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"


AFinalGameMode::AFinalGameMode() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFinalGameMode::BeginPlay() {
	Super::BeginPlay();

	bGameOver = false;
	Message = TEXT("");
	Score = 0;
	Money = 0;

	// Get the score of the previous level (or 0, if it's the first level)
	UFinalGameInstance* FinalGameInstance = Cast<UFinalGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (FinalGameInstance) {
		Score = FinalGameInstance->Score;
		Money = FinalGameInstance->MoneyCollected;

		// If the map is arches
		FString CurrentMapName = FPaths::GetBaseFilename(GetWorld()->GetMapName());

		if (CurrentMapName.Contains(TEXT("Day_1_Map"))) {
			FinalGameInstance->SetDayIndex(0);
		}

		else if (CurrentMapName.Contains(TEXT("Day_2_Map"))) {
			FinalGameInstance->SetDayIndex(1);
		}

		else if (CurrentMapName.Contains(TEXT("Day_3_Map"))) {
			FinalGameInstance->SetDayIndex(2);
		}

		else if (CurrentMapName.Contains(TEXT("Day_4_Map"))) {
			FinalGameInstance->SetDayIndex(3);
		}

		else if (CurrentMapName.Contains(TEXT("Day_5_Map"))) {
			FinalGameInstance->SetDayIndex(4);
		}

		else if (CurrentMapName.Contains(TEXT("Day_6_Map"))) {
			FinalGameInstance->SetDayIndex(5);
		}
	}

	// Set timer for LevelSeconds. Calls HandleTimeExpired when time runs out
	GetWorld()->GetTimerManager().SetTimer(LevelTimer, this, &AFinalGameMode::HandleTimeExpired, 1, true);
}

void AFinalGameMode::EndPlay(const EEndPlayReason::Type Reason) {
	GetWorld()->GetTimerManager().ClearTimer(LevelTimer);

	Super::EndPlay(Reason);
}


float AFinalGameMode::GetTimeRemaining() const {
	return LevelSeconds;
}

int32 AFinalGameMode::GetScore()
{
	return Score;
}

void AFinalGameMode::EndGame(bool bWon) {
	if (bGameOver) {
		return;
	}

	GetWorld()->GetTimerManager().PauseTimer(LevelTimer);

	bGameOver = true;

	if (bWon) {
		Score += CalculateScore();
		Message = TEXT("You completed the building!");

		// Add the score to the game instance
		UFinalGameInstance* FinalGameInstance = Cast<UFinalGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (FinalGameInstance) {
			FinalGameInstance->AddScore(Score);

			FinalGameInstance->AddMoney(Money);
		}
	}

	/*if (ResultWidgetClass) {
		UResultWidget* ResultWidget = CreateWidget<UResultWidget>(GetWorld(), ResultWidgetClass);

		if (ResultWidget) {
			ResultWidget->SetupResultScreen(bWon, Score, Message);

			// Show the widget
			ResultWidget->AddToViewport();

			if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
				PC->SetShowMouseCursor(true);

				// Swap input mode to UI only to be able to click on the widget
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(ResultWidget->TakeWidget());
				PC->SetInputMode(InputMode);
			}
		}
	}*/
}

void AFinalGameMode::HandleTimeExpired() {
	LevelSeconds--;
	if (LevelSeconds <= 0)
	{
		Message = TEXT("You ran out of time!");
		EndGame(false);
	}
}

// Needs to be implemented
int32 AFinalGameMode::CalculateScore() {
	return 0;
}

