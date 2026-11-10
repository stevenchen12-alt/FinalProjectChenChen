// FinalGameInstance.cpp


#include "FinalGameInstance.h"

void UFinalGameInstance::RestartGame() {
	MoneyCollected = 0;
	DayIndex = 0;
}

void UFinalGameInstance::NextLevel() {
	DayIndex++;

	if (DayIndex >= TotalDays) {
		RestartGame();
	}
}

void UFinalGameInstance::SetDayIndex(int32 newIndex) {
	if (newIndex >= TotalDays || newIndex < 0) {
		return;
	}

	DayIndex = newIndex;
}