// Fill out your copyright notice in the Description page of Project Settings.


#include "CutethulhuSaveGame.h"

UCutethulhuSaveGame::UCutethulhuSaveGame()
{
	SaveSlotName = TEXT("TestSaveSlot");
	UserIndex = 0;
}

void UCutethulhuSaveGame::SaveGame()
{
}

FLevelData* UCutethulhuSaveGame::GetLevelData(int levelIndex)
{
	try{
		return &LevelsData[levelIndex];
	}
	catch (const std::exception e) {
		return nullptr;
	}
}
	
