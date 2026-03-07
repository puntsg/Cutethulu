// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CutethulhuSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FLevelData {
	GENERATED_BODY()
public:
	UPROPERTY()
	FText LevelName;

	UPROPERTY()
	bool visited;

	UPROPERTY(BlueprintReadOnly)
	bool completed;

	UPROPERTY()
	TArray<bool> pickedCollectables;
};

/**
 * 
 */
UCLASS()
class CUTETHULU_API UCutethulhuSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FLevelData> LevelsData;

	UCutethulhuSaveGame();

	void SaveGame();
	FLevelData* GetLevelData(int levelIndex);

	UFUNCTION(BlueprintPure)
	bool GetIfLevelCompleted(int levelIndex);

	UFUNCTION(BlueprintCallable)
	void SetLevelAsCompleted(int levelIndex,bool value);

	UFUNCTION(BlueprintPure)
	bool GetIfCollectableIsPickedUp(int levelIndex, int collectableIndex);
};
