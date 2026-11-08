// SettingsWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class FINALPROJECTCHENCHEN_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartGame();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SettingsText;
};