// FinalGameInstance.cpp


#include "FinalGameInstance.h"

void UFinalGameInstance::ResetRun() {
	Score = 0;
	DayIndex = 0;
}

void UFinalGameInstance::NextLevel() {
	DayIndex++;

	if (DayIndex >= TotalDays) {
		ResetRun();
	}
}

void UFinalGameInstance::RestartLevel() {
	Score = PreviousScore;
	MoneyCollected = PreviousMoneyCollected;
}

void UFinalGameInstance::SetDayIndex(int32 newIndex) {
	if (newIndex >= TotalDays || newIndex < 0) {
		return;
	}

	DayIndex = newIndex;
}