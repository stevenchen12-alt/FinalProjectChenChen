// FinalGameMode.cpp

#include "FinalGameMode.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.h"
#include "SettingsWidget.h"
#include "FinalGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ReferenceBoard.h"
#include "FinalCharacter.h"
#include "ShopComponent.h"
#include "ShopWidget.h"
#include "InventoryComponent.h"


AFinalGameMode::AFinalGameMode() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShopComponent = CreateDefaultSubobject<UShopComponent>(TEXT("ShopComponent"));
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

	int32 CurrentDayIndex = 0;

	if (FinalGameInstance) {
		MoneyCollected = FinalGameInstance->MoneyCollected;

		// Get the current map name
		FString CurrentMapName = FPaths::GetBaseFilename(GetWorld()->GetMapName());

		if (CurrentMapName.Contains(TEXT("Day1Map"))) {
			CurrentDayIndex = 0;
		}

		else if (CurrentMapName.Contains(TEXT("Day2Map"))) {
			CurrentDayIndex = 1;
		}

		else if (CurrentMapName.Contains(TEXT("Day3Map"))) {
			CurrentDayIndex = 2;
		}

		else if (CurrentMapName.Contains(TEXT("Day4Map"))) {
			CurrentDayIndex = 3;
		}

		else if (CurrentMapName.Contains(TEXT("Day5Map"))) {
			CurrentDayIndex = 4;
		}

		else if (CurrentMapName.Contains(TEXT("Day6Map"))) {
			CurrentDayIndex = 5;
		}

		FinalGameInstance->SetDayIndex(CurrentDayIndex);
	}

	// Find PlayerBuildPad and randomly pick a template from the list of today's templates
	BuildPad = Cast<APlayerBuildPad>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerBuildPad::StaticClass()));

	if (BuildPad) {
		if (DayTemplates.IsValidIndex(CurrentDayIndex)) {
			TArray<TObjectPtr<UStructureTemplate>>& List = DayTemplates[CurrentDayIndex].Templates;

			if (List.Num() > 0) {
				int32 Index = FMath::RandRange(0, List.Num() - 1);
				BuildPad->ActiveTemplate = List[Index];

				TArray<AActor*> FoundBoards;

				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AReferenceBoard::StaticClass(), FoundBoards);

				for (AActor* BoardActor : FoundBoards) {
					if (AReferenceBoard* Board = Cast<AReferenceBoard>(BoardActor)) {
						Board->UpdateDisplayedImage();
					}
				}
			}

			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("AFinalGameMode: Template pool for Day %d is empty!"), CurrentDayIndex + 1));
			}
		}
	}

	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("AFinalGameMode: No BuildPad found in level!"));
	}

	// Set timer for LevelSeconds. Calls HandleTimeExpired when time runs out
	GetWorld()->GetTimerManager().SetTimer(LevelTimer, this, &AFinalGameMode::HandleTimeExpired, 1, true);
}

void AFinalGameMode::EndPlay(const EEndPlayReason::Type Reason) {
	GetWorld()->GetTimerManager().ClearTimer(LevelTimer);

	Super::EndPlay(Reason);
}

void AFinalGameMode::SubmitBuildResult(const FBuildEvaluationResult& Result) {
	if (bGameOver) {
		return;
	}

	LastEvaluation = Result;

	if (Result.Accuracy >= MinAccuracyThreshold && Result.NumCorrectBlocks > 0) {
		EndGame(true);
	}

	else {
		EndGame(false);
	}
}


float AFinalGameMode::GetTimeRemaining() const {
	return LevelSeconds;
}

int32 AFinalGameMode::GetScore() const 
{
	return Score;
}

int32 AFinalGameMode::GetMoneyCollected() const {
	return MoneyCollected;
}

bool AFinalGameMode::SpendMoney(int32 Amount) {
	if (Amount < 0 || GetMoneyCollected() < Amount) {
		return false;
	}

	MoneyCollected -= Amount;
	OnMoneyChanged.Broadcast(MoneyCollected);

	return true;
}

void AFinalGameMode::EndGame(bool bWon) {
	if (bGameOver) {
		return;
	}

	GetWorld()->GetTimerManager().PauseTimer(LevelTimer);

	bGameOver = true;

	Score = CalculateScore();
	MoneyCollected = CalculateMoneyCollected();

	if (bWon) {
		if (LastEvaluation.bIsPerfectMatch) {
			Message = TEXT("Perfect build! 100% correct!");
		}

		else {
			Message = FString::Printf(TEXT("You completed the building! %d/%d blocks correct!"), LastEvaluation.NumCorrectBlocks, LastEvaluation.TotalTargetBlocks);
		}
		
	}

	else if (Message.IsEmpty()) {
		Message = FString::Printf(TEXT("You Lose! Only %d/%d blocks were correct!"), LastEvaluation.NumCorrectBlocks, LastEvaluation.TotalTargetBlocks);
	}

	if (ResultWidgetClass) {
		UResultWidget* ResultWidget = CreateWidget<UResultWidget>(GetWorld(), ResultWidgetClass);

		if (ResultWidget) {
			ResultWidget->SetupResultScreen(bWon, Score, MoneyCollected, LastEvaluation.Accuracy, Message);

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
		if (BuildPad) {
			LastEvaluation = BuildPad->EvaluateBuild();
		}

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

			// Bind 'p' to ToggleShop() through the player's input component
			IC->BindKey(EKeys::P, IE_Pressed, this, &AFinalGameMode::ToggleShop);
		}
	}
}

void AFinalGameMode::NextLevel() {
	UFinalGameInstance* FinalGameInstance = Cast<UFinalGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (FinalGameInstance) {
		FinalGameInstance->SaveMoney(MoneyCollected);

		if (AFinalCharacter* Player = Cast<AFinalCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0))) {
			if (UInventoryComponent* InventoryComponent = Player->FindComponentByClass<UInventoryComponent>()) {
				FinalGameInstance->SavePowerups(InventoryComponent->PurchasedItems);
			}
		}
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

int32 AFinalGameMode::CalculateScore() {
	int32 Points = LastEvaluation.NumCorrectBlocks * PointsPerCorrectBlock;
	int32 Penalty = LastEvaluation.NumWrongBlocks * PenaltyPerWrongBlock;

	return FMath::Max(0, Points - Penalty);
}

int32 AFinalGameMode::CalculateMoneyCollected() {
	return FMath::RoundToInt(LastEvaluation.Accuracy * (Score / 100.0f));
}

void AFinalGameMode::ToggleShop() {
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	if (!PC) {
		return;
	}

	// Close the shop if it's already open
	if (ActiveShopWidget && ActiveShopWidget->IsInViewport()) {
		ActiveShopWidget->CloseShop();
		ActiveShopWidget = nullptr;

		return;
	}

	// Get the inventory component
	APawn* PlayerPawn = PC->GetPawn();
	
	if (PlayerPawn) {
		UInventoryComponent* InventoryComponent = PlayerPawn->FindComponentByClass<UInventoryComponent>();

		if (ShopWidgetClass && ShopComponent && InventoryComponent) {
			ActiveShopWidget = CreateWidget<UShopWidget>(PC, ShopWidgetClass);

			if (ActiveShopWidget) {
				ActiveShopWidget->AddToViewport();

				ActiveShopWidget->InitializeShop(ShopComponent, InventoryComponent);

				FInputModeGameAndUI InputMode;
				InputMode.SetWidgetToFocus(ActiveShopWidget->TakeWidget());
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;
			}
		}
	}
	
}