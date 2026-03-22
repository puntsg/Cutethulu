// Fill out your copyright notice in the Description page of Project Settings.


#include "YakotakiSaveGame.h"
#include <Kismet/GameplayStatics.h>

UYakotakiSaveGame::UYakotakiSaveGame()
{
	SaveSlotName = TEXT("player");
	UserIndex = 0;
}

void UYakotakiSaveGame::SaveGame()
{
	UGameplayStatics::SaveGameToSlot(this, SaveSlotName, UserIndex);
}

FLevelData* UYakotakiSaveGame::GetLevelData(int levelIndex)
{
	if(levelIndex >= 0 && levelIndex < LevelsData.Num())
		return &LevelsData[levelIndex];
	return nullptr;
	
}

bool UYakotakiSaveGame::GetIfLevelCompleted(int levelIndex)
{
	if (levelIndex >= 0 && levelIndex < LevelsData.Num()) {
		FLevelData* selectedLevelData = &LevelsData[levelIndex];
		return selectedLevelData->completed;
	}
	return false;
}

int UYakotakiSaveGame::GetPickedCollectables(int levelIndex)
{
	int picked = 0;
	if (levelIndex >= 0 && levelIndex < LevelsData.Num()) {
		FLevelData* selectedLevelData = &LevelsData[levelIndex];
		for (int i = 0; i < selectedLevelData->pickedCollectables.Num(); i++)
			if (selectedLevelData->pickedCollectables[i])
				picked++;
	}
	return picked;
}

void UYakotakiSaveGame::SetLevelAsCompleted(int levelIndex, bool value)
{
	while (this->LevelsData.Num() <= levelIndex)
	{
		FLevelData newLevelData;
		newLevelData.completed = false;
		this->LevelsData.Add(newLevelData);
	}
	this->LevelsData[levelIndex].completed = value;
	SaveGame();
}

void UYakotakiSaveGame::DeleteLevelData(int levelIndex)
{
	while (this->LevelsData.Num() <= levelIndex)
	{
		FLevelData newLevelData;
		newLevelData.completed = false;
		this->LevelsData.Add(newLevelData);
	}
	this->LevelsData[levelIndex].pickedCollectables.Empty();
	this->LevelsData[levelIndex].completed = false;
	this->LevelsData[levelIndex].visited = false;
	SaveGame();
}

bool UYakotakiSaveGame::GetIfCollectableIsPickedUp(int levelIndex, int collectableIndex)
{
	if (levelIndex >= 0 && levelIndex < LevelsData.Num()) {
		FLevelData* selectedLevelData = &LevelsData[levelIndex];
		if (collectableIndex >= 0 && collectableIndex < selectedLevelData->pickedCollectables.Num()) {
			return selectedLevelData->pickedCollectables[collectableIndex];
		}
	}
	return false;
}
	
