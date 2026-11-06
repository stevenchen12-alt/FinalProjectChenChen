// ResultWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class FINALPROJECTCHENCHEN_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupResultScreen(bool bWon, int32 Score, int32 MoneyCollected, FString& Message);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void NextLevel();

	UFUNCTION()
	void RestartLevel();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NextButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartLevelButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultTitleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultText;
};