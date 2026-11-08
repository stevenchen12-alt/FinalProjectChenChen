// SettingsWidget.cpp


#include "SettingsWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void USettingsWidget::NativeConstruct() {
	Super::NativeConstruct();

	// Connect the start game button to StartGame()
	if (StartGameButton) {
		StartGameButton->OnClicked.AddDynamic(this, &USettingsWidget::StartGame);
	}
}

void USettingsWidget::StartGame() {
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0)) {
		PC->SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}