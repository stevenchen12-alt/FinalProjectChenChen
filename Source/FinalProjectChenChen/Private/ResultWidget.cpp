// ResultWidget.cpp


#include "ResultWidget.h"
#include "FinalGameInstance.h"
#include "FinalGameMode.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Spacer.h"
#include "Kismet/GameplayStatics.h"

void UResultWidget::NativeConstruct() {
	Super::NativeConstruct();

	// Connect the restart game button to RestartGame()
	if (RestartGameButton) {
		RestartGameButton->OnClicked.AddDynamic(this, &UResultWidget::RestartGame);
	}

	// Connect the next button to NextLevel()
	if (NextButton) {
		NextButton->OnClicked.AddDynamic(this, &UResultWidget::NextLevel);
	}

	// Connect the restart level button to RestartLevel()
	if (RestartLevelButton) {
		RestartLevelButton->OnClicked.AddDynamic(this, &UResultWidget::RestartLevel);
	}
}

void UResultWidget::SetupResultScreen(bool bWon, int32 Score, int32 MoneyCollected, FString& Message) {
	if (ResultTitleText) {
		if (bWon) {
			ResultTitleText->SetText(FText::FromString("You Win!"));

			UFinalGameInstance* FinalGameInstance = Cast<UFinalGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			int32 DayIndex = -1;

			if (FinalGameInstance) {
				DayIndex = FinalGameInstance->DayIndex;
			}

			if (DayIndex >= 0) {
				// If it's the last day, show RestartGameButton and RestartLevelButton
				if (DayIndex == 5) {
					if (RestartGameButton) {
						RestartGameButton->SetVisibility(ESlateVisibility::Visible);
					}

					if (NextButton) {
						NextButton->SetVisibility(ESlateVisibility::Collapsed);
					}

					if (RestartLevelButton) {
						RestartLevelButton->SetVisibility(ESlateVisibility::Visible);
					}
				}

				// Any other day, show RestartLevelButton and NextLevelButton
				else {
					if (RestartGameButton) {
						RestartGameButton->SetVisibility(ESlateVisibility::Collapsed);
					}

					if (NextButton) {
						NextButton->SetVisibility(ESlateVisibility::Visible);
					}

					if (RestartLevelButton) {
						RestartLevelButton->SetVisibility(ESlateVisibility::Visible);
					}
				}
			}
		}

		else {
			ResultTitleText->SetText(FText::FromString("You Lost!"));

			// Only show RestartLevelButton
			if (RestartGameButton) {
				RestartGameButton->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (NextButton) {
				NextButton->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (RestartLevelButton) {
				RestartLevelButton->SetVisibility(ESlateVisibility::Visible);
			}

			// Don't show the spacers, so the restart level button is centered
			if (Spacer1) {
				Spacer1->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (Spacer2) {
				Spacer2->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}

	if (ResultText) {
		Message += TEXT("\n\nScore: ");
		Message += FString::FromInt(Score);

		Message += TEXT("\n\nMoney Collected: ");
		Message += FString::FromInt(MoneyCollected);

		ResultText->SetText(FText::FromString(Message));
	}
}

void UResultWidget::RestartGame() {
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0)) {
		PC->SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	FName NextLevelName = TEXT("/Game/Maps/Day1Map");
	UGameplayStatics::OpenLevel(this, NextLevelName, false);
}

void UResultWidget::NextLevel() {
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0)) {
		PC->SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	// Cast the current game mode to FinalGameMode
	if (AFinalGameMode* GameMode = Cast<AFinalGameMode>(GetWorld()->GetAuthGameMode())) {
		GameMode->NextLevel();
	}

	FName NextLevelName = NAME_None;

	UFinalGameInstance* FinalGameInstance = Cast<UFinalGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	
	if (FinalGameInstance) {
		FinalGameInstance->NextLevel();

		switch (FinalGameInstance->DayIndex) {
		case 0:
			NextLevelName = TEXT("/Game/Maps/Day1Map");
			break;

		case 1:
			NextLevelName = TEXT("/Game/Maps/Day2Map");
			break;

		case 2:
			NextLevelName = TEXT("/Game/Maps/Day3Map");
			break;

		case 3:
			NextLevelName = TEXT("/Game/Maps/Day4Map");
			break;

		case 4:
			NextLevelName = TEXT("/Game/Maps/Day5Map");
			break;

		case 5:
			NextLevelName = TEXT("/Game/Maps/Day6Map");
			break;
		}

		if (!NextLevelName.IsNone()) {
			UGameplayStatics::OpenLevel(this, NextLevelName, false);
		}

		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Cannot open the next level!"));
		}
	}
}

void UResultWidget::RestartLevel() {
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0)) {
		PC->SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

