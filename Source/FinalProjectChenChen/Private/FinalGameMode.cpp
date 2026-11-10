// FinalGameMode.cpp

#include "FinalGameMode.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.h"
#include "SettingsWidget.h"
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

	UFinalGameInstance* FinalGameInstance = Cast<UFinalGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (FinalGameInstance) {
		if (FinalGameInstance->bFirstLoad) {
			FinalGameInstance->bFirstLoad = false;

			if (SettingsWidgetClass) {
				USettingsWidget* SettingsWidget = CreateWidget<USettingsWidget>(GetWorld(), SettingsWidgetClass);

				if (SettingsWidget) {
					// Show the widget
					SettingsWidget->AddToViewport();

					if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
						PC->SetShowMouseCursor(true);

						// Swap input mode to UI only to be able to click on the widget
						FInputModeUIOnly InputMode;
						InputMode.SetWidgetToFocus(SettingsWidget->TakeWidget());
						PC->SetInputMode(InputMode);
					}
				}
			}
		}
	}

	bGameOver = false;
	Message = TEXT("");
	Score = 0;
	MoneyCollected = 0;

	if (FinalGameInstance) {
		MoneyCollected = FinalGameInstance->MoneyCollected;

		// If the map is arches
		FString CurrentMapName = FPaths::GetBaseFilename(GetWorld()->GetMapName());

		if (CurrentMapName.Contains(TEXT("Day1Map"))) {
			FinalGameInstance->SetDayIndex(0);
		}

		else if (CurrentMapName.Contains(TEXT("Day2Map"))) {
			FinalGameInstance->SetDayIndex(1);
		}

		else if (CurrentMapName.Contains(TEXT("Day3Map"))) {
			FinalGameInstance->SetDayIndex(2);
		}

		/*else if (CurrentMapName.Contains(TEXT("Day4Map"))) {
			FinalGameInstance->SetDayIndex(3);
		}

		else if (CurrentMapName.Contains(TEXT("Day5Map"))) {
			FinalGameInstance->SetDayIndex(4);
		}

		else if (CurrentMapName.Contains(TEXT("Day6Map"))) {
			FinalGameInstance->SetDayIndex(5);
		}*/
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

int32 AFinalGameMode::GetMoneyCollected() {
	return MoneyCollected;
}

void AFinalGameMode::EndGame(bool bWon) {
	if (bGameOver) {
		return;
	}

	GetWorld()->GetTimerManager().PauseTimer(LevelTimer);

	bGameOver = true;

	if (bWon) {
		Score = CalculateScore();
		Message = TEXT("You completed the building!");
	}

	if (ResultWidgetClass) {
		UResultWidget* ResultWidget = CreateWidget<UResultWidget>(GetWorld(), ResultWidgetClass);

		if (ResultWidget) {
			ResultWidget->SetupResultScreen(bWon, Score, MoneyCollected, Message);

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
	}
}

void AFinalGameMode::HandleTimeExpired() {
	LevelSeconds--;
	if (LevelSeconds <= 0)
	{
		Message = TEXT("You ran out of time!");
		EndGame(false);
	}
}

void AFinalGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) {
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (NewPlayer) {
		if (UInputComponent* IC = NewPlayer->InputComponent) {
			// Bind 'z' to DebugWinLevel() through the player's input component
			IC->BindKey(EKeys::Z, IE_Pressed, this, &AFinalGameMode::DebugWinLevel);

			// Bind 'x' to DebugLoseLevel() through the player's input component
			IC->BindKey(EKeys::X, IE_Pressed, this, &AFinalGameMode::DebugLoseLevel);

			// Bind 'c' to DebugWinLevelLast() through the player's input component
			IC->BindKey(EKeys::C, IE_Pressed, this, &AFinalGameMode::DebugWinLevelLast);
		}
	}
}

void AFinalGameMode::NextLevel() {
	UFinalGameInstance* FinalGameInstance = Cast<UFinalGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (FinalGameInstance) {
		FinalGameInstance->SaveMoney(MoneyCollected);
	}
}

void AFinalGameMode::DebugWinLevel() {
	EndGame(true);
}

void AFinalGameMode::DebugLoseLevel() {
	EndGame(false);
}

void AFinalGameMode::DebugWinLevelLast() {
	UFinalGameInstance* FinalGameInstance = Cast<UFinalGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (FinalGameInstance) {
		FinalGameInstance->SetDayIndex(FinalGameInstance->TotalDays-1);
	}

	EndGame(true);
}

// Needs to be implemented
int32 AFinalGameMode::CalculateScore() {
	return 0;
}

