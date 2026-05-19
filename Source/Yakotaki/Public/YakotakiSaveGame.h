// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "YakotakiSaveGame.generated.h"

#pragma region DataStructs
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
	bool replaying;

	UPROPERTY(BlueprintReadOnly)
	TArray<bool> pickedCollectables;

	UPROPERTY(BlueprintReadOnly)
	TArray<bool> hasCollectableBeenChecked;

	UPROPERTY(BlueprintReadOnly)
	bool hasImageBeenChecked = false;
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
#pragma endregion DataStucts

UCLASS()
class YAKOTAKI_API UYakotakiSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
#pragma region Vars
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(BlueprintReadWrite, Category = Basic)
	int collectedParticles;

	UPROPERTY(BlueprintReadWrite, Category = Basic)
	TArray<FTutorialData> TutorialsData;

	UPROPERTY(BlueprintReadWrite, Category = Basic)
	TArray<FLevelData> LevelsData;
#pragma endregion Vars
	UYakotakiSaveGame();
	void SaveGame();
	FLevelData* GetLevelData(int levelIndex);
	UFUNCTION(BlueprintPure)
	bool GetIfLevelCompleted(int levelIndex);
	UFUNCTION(BlueprintCallable)
	int GetPickedCollectablesNum(int levelIndex);
	UFUNCTION(BlueprintCallable)
	void SetLevelAsCompleted(int levelIndex,bool value, bool autoSave);
	UFUNCTION(BlueprintCallable)
	void SetLevelAsReplaying(int levelIndex, bool value, bool autoSave);
	UFUNCTION(BlueprintCallable)
	void DeleteLevelData(int levelIndex, bool autoSave);
	UFUNCTION(BlueprintPure)
	bool GetIfCollectableIsPickedUp(int levelIndex, int collectableIndex);
	UFUNCTION(BlueprintPure)
	bool GetIfPickedUpCollectableChecked(int levelIndex, int collectableIndex);
	UFUNCTION(BlueprintCallable)
	void MarkCollectableAsChecked(int levelIndex, int collectableIndex, bool autoSave);
	UFUNCTION(BlueprintPure)
	bool GetIfCollectablesImageChecked(int levelIndex);
	UFUNCTION(BlueprintCallable)
	void MarkCollectablesImageAsChecked(int levelIndex, bool autoSave);

	UFUNCTION(BlueprintCallable)
	bool GetIfTutorialCompleted(FString tutorialID);
	UFUNCTION(BlueprintCallable)
	void SetTutorialAsCompleted(FString tutorialID, bool autoSave);

	UFUNCTION(BlueprintCallable)
	void SaveCollectedParticles(int collected, bool autoSave);

	UFUNCTION(BlueprintCallable)
	void SaveCollectable(int levelIndex, int CollectableID, bool autoSave);

	UFUNCTION(BlueprintCallable)
	bool IsLevelCompleted(int levelIndex);
	UFUNCTION(BlueprintCallable)
	bool IsBeingReplayed(int levelIndex);
	
	UFUNCTION(BlueprintCallable)
	bool IsCollectablePickedUp(int levelIndex, int CollectableID);
	
	UFUNCTION(BlueprintCallable)
	TArray<bool> GetPickedCollectables(int levelIndex);
private:
	void UpdateTutorialsData();
};
