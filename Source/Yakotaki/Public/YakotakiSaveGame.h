// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "YakotakiSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FLevelData {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FText LevelName;

	UPROPERTY(BlueprintReadOnly)
	bool visited;

	UPROPERTY(BlueprintReadOnly)
	bool completed;

	UPROPERTY(BlueprintReadOnly)
	TArray<bool> pickedCollectables;

	UPROPERTY(BlueprintReadOnly)
	TArray<bool> hasCollectableBeenChecked;

	UPROPERTY(BlueprintReadOnly)
	bool hasImageBeenChecked;
};

USTRUCT(BlueprintType)
struct FTutorialData {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FString id; 

	UPROPERTY(BlueprintReadOnly)
	bool completed;
};

UCLASS()
class YAKOTAKI_API UYakotakiSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	//Save data IDs
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	//Game saved data
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FTutorialData> TutorialsData;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FLevelData> LevelsData;

	UYakotakiSaveGame();
	void SaveGame();
	FLevelData* GetLevelData(int levelIndex);
	UFUNCTION(BlueprintPure)
	bool GetIfLevelCompleted(int levelIndex);
	UFUNCTION(BlueprintCallable)
	int GetPickedCollectables(int levelIndex);
	UFUNCTION(BlueprintCallable)
	void SetLevelAsCompleted(int levelIndex,bool value);
	UFUNCTION(BlueprintCallable)
	void DeleteLevelData(int levelIndex);
	UFUNCTION(BlueprintPure)
	bool GetIfCollectableIsPickedUp(int levelIndex, int collectableIndex);
	UFUNCTION(BlueprintPure)
	bool GetIfPickedUpCollectableChecked(int levelIndex, int collectableIndex);
	UFUNCTION(BlueprintCallable)
	void MarkCollectableAsChecked(int levelIndex, int collectableIndex);
	UFUNCTION(BlueprintPure)
	bool GetIfCollectablesImageChecked(int levelIndex);
	UFUNCTION(BlueprintCallable)
	void MarkCollectablesImageAsChecked(int levelIndex);

	UFUNCTION(BlueprintCallable)
	bool GetIfTutorialCompleted(FString tutorialID);
	UFUNCTION(BlueprintCallable)
	void SetTutorialAsCompleted(FString tutorialID);
private:
	void UpdateTutorialsData();
};
