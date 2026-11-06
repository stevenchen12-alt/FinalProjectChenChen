// FinalGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FinalGameMode.generated.h"

class UResultWidget;

UCLASS()
class FINALPROJECTCHENCHEN_API AFinalGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AFinalGameMode();

    UFUNCTION(BlueprintPure, Category = "Rules")
    float GetTimeRemaining() const;

    UFUNCTION(BlueprintPure, Category = "Rules")
    int32 GetScore();

    UFUNCTION(BlueprintPure, Category = "Rules")
    int32 CalculateScore();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;

    void EndGame(bool bWon);
    void HandleTimeExpired();

    UPROPERTY(EditDefaultsOnly, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
    float LevelSeconds = 300.f;

    /** Assign your WBP_Result child of this on BP_FinalGameMode. */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UResultWidget> ResultWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "Rules")
    int32 Score = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Rules")
    int32 MoneyCollected = 0;

    bool bGameOver = false;
    FTimerHandle LevelTimer;

    FString Message;
};