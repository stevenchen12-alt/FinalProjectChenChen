// FinalGameInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FinalGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECTCHENCHEN_API UFinalGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Run")
    int32 MoneyCollected = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Run")
    int32 TotalDays = 3;//6;

    /** Index 0 for Day 1
      * Index 1 for Day 2
      * Index 2 for Day 3
      * Index 3 for Day 4
      * Index 4 for Day 5
      * Index 5 for Day 6
    */
    UPROPERTY(BlueprintReadOnly, Category = "Run")
    int32 DayIndex = 0;

    UFUNCTION(BlueprintCallable, Category = "Run")
    void SaveMoney(int32 NewMoneyCollected) { MoneyCollected = NewMoneyCollected; }

    /** Call this when the player chooses Play Again. */
    UFUNCTION(BlueprintCallable, Category = "Run")
    void RestartGame();

    UFUNCTION(BlueprintCallable, Category = "Run")
    void NextLevel();

    /** Call this when the player chooses Restart Level. */
    //UFUNCTION(BlueprintCallable, Category = "Run")
    //void RestartLevel();

    UFUNCTION(BlueprintCallable, Category = "Run")
    void SetDayIndex(int32 newIndex);

    bool bFirstLoad = true;
};
