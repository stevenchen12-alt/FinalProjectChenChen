// FinalGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerBuildPad.h"
#include "FinalGameMode.generated.h"

class UResultWidget;
class USettingsWidget;
class UStructureTemplate;
class UShopComponent;
class UShopWidget;

// Wrapper for list of templates for each day
USTRUCT(BlueprintType) 
struct FDayTemplateList {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Templates")
    TArray<TObjectPtr<UStructureTemplate>> Templates;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewAmount);

UCLASS()
class FINALPROJECTCHENCHEN_API AFinalGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AFinalGameMode();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;
    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

    // Called directly from HUD's done button after evaluating PlayerBuildPad
    UFUNCTION(BlueprintCallable, Category = "Score")
    void SubmitBuildResult(const FBuildEvaluationResult& Result);

    UFUNCTION(BlueprintPure, Category = "Rules")
    float GetTimeRemaining() const;

    UFUNCTION(BlueprintPure, Category = "Rules")
    int32 GetScore() const;

    UFUNCTION(BlueprintPure, Category = "Money")
    int32 GetMoneyCollected() const;

    UPROPERTY(BlueprintAssignable, Category = "Money")
    FOnMoneyChanged OnMoneyChanged;

    UFUNCTION(BlueprintCallable, Category = "Money")
    bool SpendMoney(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Rules")
    void EndGame(bool bWon);

    UFUNCTION(BlueprintCallable, Category = "Rules")
    void NextLevel();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop")
    TObjectPtr<UShopComponent> ShopComponent;

    UFUNCTION(BlueprintCallable, Category = "Shop")
    void ToggleShop();

protected:
    // Index 0 = Day 1
    // Index 1 = Day 2
    // Index 2 = Day 3
    // Index 3 = Day 4
    // Index 4 = Day 5
    // Index 5 = Day 6
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Templates")
    TArray<FDayTemplateList> DayTemplates;

    // Scoring rules
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score")
    int32 PointsPerCorrectBlock = 100;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score")
    int32 PenaltyPerWrongBlock = 50;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float MinAccuracyThreshold = 75.0f;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UResultWidget> ResultWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USettingsWidget> SettingsWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
    float LevelSeconds = 300.f;

    UPROPERTY(BlueprintReadOnly, Category = "Rules")
    bool bGameOver = false;

    UPROPERTY(BlueprintReadOnly, Category = "Rules")
    FString Message;

    UPROPERTY(BlueprintReadOnly, Category = "Rules")
    int32 Score = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Rules")
    int32 MoneyCollected = 0;

    // Stores evaluation reslt passed from HUD button
    UPROPERTY(BlueprintReadOnly, Category = "Scoring")
    FBuildEvaluationResult LastEvaluation;

    UPROPERTY(EditDefaultsOnly, Category = "Shop")
    TSubclassOf<UShopWidget> ShopWidgetClass;
    
private:
    FTimerHandle LevelTimer;

    UPROPERTY()
    TObjectPtr<APlayerBuildPad> BuildPad;

    UPROPERTY()
    TObjectPtr<UShopWidget> ActiveShopWidget;

    void HandleTimeExpired();
    int32 CalculateScore();
    int32 CalculateMoneyCollected();

    void DebugWinLevel();
    void DebugLoseLevel();
    void DebugWinLevelLast();
};