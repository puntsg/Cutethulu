// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "DualLevel.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHorrorMapLoad);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHorrorMapUnload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCuteMapLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCuteMapUnloaded);
UCLASS()
class CUTETHULU_API ADualLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Streaming")
	TSoftObjectPtr<UWorld> corruptedLevel;
	UPROPERTY(EditAnywhere, Category = "Streaming")
	TSoftObjectPtr<UWorld> healedAsset;

	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnLevelLoaded OnLevelLoaded;

	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnMapChange OnMapChange;

	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnHorrorMapLoad OnHorrorMapLoad;

	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnHorrorMapLoad OnHorrorMapUnload;

	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnCuteMapLoaded OnCuteMapLoaded;

	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnCuteMapLoaded OnCuteMapUnloaded;

	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void UnloadStreamedLevels();

	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void LoadHorrorLevel();

	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void UnloadHorrorLevel();

	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void LoadCuteLevel();

	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void UnloadCuteLevel();

	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void SwapLevel();
protected:
	virtual void BeginPlay() override;

};
