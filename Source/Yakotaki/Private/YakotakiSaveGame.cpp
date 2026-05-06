// Fill out your copyright notice in the Description page of Project Settings.


#include "YakotakiSaveGame.h"
#include "Engine/DataTable.h"
#include "TutorialEntry.h"
#include <Kismet/GameplayStatics.h>

UYakotakiSaveGame::UYakotakiSaveGame()
{
	SaveSlotName = TEXT("player");
	UserIndex = 0;
	UpdateTutorialsData();
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

void UYakotakiSaveGame::SetLevelAsCompleted(int levelIndex, bool value, bool autoSave)
{
	while (this->LevelsData.Num() <= levelIndex)
	{
		FLevelData newLevelData;
		newLevelData.completed = false;
		this->LevelsData.Add(newLevelData);
	}
	this->LevelsData[levelIndex].completed = value;
	if(autoSave)
		SaveGame();
}

void UYakotakiSaveGame::DeleteLevelData(int levelIndex, bool autoSave)
{
	while (this->LevelsData.Num() <= levelIndex)
	{
		FLevelData newLevelData;
		newLevelData.completed = false;
		this->LevelsData.Add(newLevelData);
	}
	this->LevelsData[levelIndex].pickedCollectables.Empty();
	this->LevelsData[levelIndex].hasCollectableBeenChecked.Empty();
	this->LevelsData[levelIndex].completed = false;
	this->LevelsData[levelIndex].visited = false;
	if(autoSave)
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

bool UYakotakiSaveGame::GetIfPickedUpCollectableChecked(int levelIndex, int collectableIndex)
{
	if (levelIndex >= 0 && levelIndex < LevelsData.Num()) {
		FLevelData* selectedLevelData = &LevelsData[levelIndex];
		if (collectableIndex >= 0 && collectableIndex < selectedLevelData->hasCollectableBeenChecked.Num()) {
			return selectedLevelData->hasCollectableBeenChecked[collectableIndex];
		}
	}
	return false;
}

void UYakotakiSaveGame::MarkCollectableAsChecked(int levelIndex, int collectableIndex,bool autoSave)
{
	if (levelIndex >= 0 && levelIndex < LevelsData.Num()) {
		FLevelData* selectedLevelData = &LevelsData[levelIndex];
		if (collectableIndex >= 0 && collectableIndex < selectedLevelData->hasCollectableBeenChecked.Num()) {
			selectedLevelData->hasCollectableBeenChecked[collectableIndex] = true;
		}
	}
}

bool UYakotakiSaveGame::GetIfCollectablesImageChecked(int levelIndex)
{
	if (levelIndex >= 0 && levelIndex < LevelsData.Num()) {
		FLevelData* selectedLevelData = &LevelsData[levelIndex];
		return selectedLevelData->hasImageBeenChecked;
	}
	return false;
}

void UYakotakiSaveGame::MarkCollectablesImageAsChecked(int levelIndex, bool autoSave)
{
	if (levelIndex >= 0 && levelIndex < LevelsData.Num()) {
		FLevelData* selectedLevelData = &LevelsData[levelIndex];
		selectedLevelData->hasImageBeenChecked = true;
	}
}

bool UYakotakiSaveGame::GetIfTutorialCompleted(FString tutorialID)
{ 
	for (int i = 0; i < TutorialsData.Num(); i++)
		if (TutorialsData[i].id.Equals(tutorialID))
			return TutorialsData[i].completed;
	return false;
}

void UYakotakiSaveGame::SetTutorialAsCompleted(FString tutorialID, bool autoSave)
{
	for (int i = 0; i < TutorialsData.Num(); i++) {
		if (TutorialsData[i].id.Equals(tutorialID)) {
			TutorialsData[i].completed = true;
			break;
		}
	}
}

void UYakotakiSaveGame::SaveCollectedParticles(int collected, bool autoSave)
{
	this->collectedParticles = collected;
	SaveGame();
}

void UYakotakiSaveGame::UpdateTutorialsData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Game/Project/00_Generic/Blueprints/Tables/DT_TutorialsEntries"));
	if (DataTableFinder.Succeeded()) {
		UDataTable* tutorialEntriesTable = DataTableFinder.Object;
		TArray<FTutorialEntry*> tutorialRows;
		tutorialEntriesTable->GetAllRows<FTutorialEntry>(TEXT(""), tutorialRows);
		for (FTutorialEntry* Row : tutorialRows) {
			bool alreadyContainsEntry = false; 
			for (FTutorialData data : TutorialsData)
				if (data.id == Row->tutorialName)
					alreadyContainsEntry = true;
			
			if (!alreadyContainsEntry) {
				FTutorialData newTutorialData;
				newTutorialData.id = Row->tutorialName;
				newTutorialData.completed = false;
				TutorialsData.Add(newTutorialData);
			}
		}
	}
}
	
