// Fill out your copyright notice in the Description page of Project Settings.


#include "CutethulhuSaveGame.h"
#include <Kismet/GameplayStatics.h>

UCutethulhuSaveGame::UCutethulhuSaveGame()
{
	SaveSlotName = TEXT("player");
	UserIndex = 0;
}

void UCutethulhuSaveGame::SaveGame()
{
	UGameplayStatics::SaveGameToSlot(this, SaveSlotName, UserIndex);
}

FLevelData* UCutethulhuSaveGame::GetLevelData(int levelIndex)
{
	if(levelIndex >= 0 && levelIndex < LevelsData.Num())
		return &LevelsData[levelIndex];
	return nullptr;
	
}

bool UCutethulhuSaveGame::GetIfLevelCompleted(int levelIndex)
{
	if (levelIndex >= 0 && levelIndex < LevelsData.Num()) {
		FLevelData* selectedLevelData = &LevelsData[levelIndex];
		return selectedLevelData->completed;
	}
	return false;
}

void UCutethulhuSaveGame::SetLevelAsCompleted(int levelIndex, bool value)
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

void UCutethulhuSaveGame::DeleteLevelData(int levelIndex)
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

bool UCutethulhuSaveGame::GetIfCollectableIsPickedUp(int levelIndex, int collectableIndex)
{
	if (levelIndex >= 0 && levelIndex < LevelsData.Num()) {
		FLevelData* selectedLevelData = &LevelsData[levelIndex];
		if (collectableIndex >= 0 && collectableIndex < selectedLevelData->pickedCollectables.Num()) {
			return selectedLevelData->pickedCollectables[collectableIndex];
		}
	}
	return false;
}
	
