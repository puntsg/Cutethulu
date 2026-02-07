// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/LevelStreamingDynamic.h"
#include "DualLevel.generated.h"

UENUM(BlueprintType)
enum class ELoaded : uint8 {
	NONE	UMETA(DisplayName = "None"),
	CUTE UMETA(DisplayName = "Cute"),
	HORROR UMETA(DisplayName = "Horror"),
	BOTH UMETA(DisplayName = "Both")
};

UENUM()
enum class ESwapEvent : uint8 {
	HorrorLoaded,
	HorrorUnloaded,
	CuteLoaded,
	CuteUnloaded,
	BothLoaded,
	BothUnloaded,
	Swapped
};

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

	UPROPERTY(EditAnywhere)
	int LevelID;

	UPROPERTY(EditAnywhere)
	FText LevelName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int numOfCollectables;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Streaming", meta = (ToolTip = "Si al cargar el nivel principal se quiere que se aplique el estado definido en loadedState  (el valor de abajo creo)"))
	bool overrideLoadedState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Streaming", meta = (ToolTip = "Valor de que está cargado"))
	ELoaded loadedState;

	UPROPERTY(EditAnywhere, Category = "Streaming")
	TSoftObjectPtr<UWorld> horrorLevel;
	UPROPERTY(EditAnywhere, Category = "Streaming")
	TSoftObjectPtr<UWorld> cuteLevel;

	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnLevelLoaded OnLevelLoaded;

	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnMapChange OnMapChange;

	// Horror - fires immediately when load starts
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnHorrorMapLoad OnHorrorMapLoad;

	// Horror - fires when level is fully loaded (actors ready)
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnHorrorMapLoad OnHorrorMapLoaded;

	// Horror - fires on unload start
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnHorrorMapLoad OnHorrorMapUnload;

	// Horror - fires when unload completes
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnHorrorMapLoad OnHorrorMapUnloaded;

	// Cute - fires immediately when load starts
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnCuteMapLoaded OnCuteMapLoad;

	// Cute - fires when level is fully loaded (actors ready)
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnCuteMapLoaded OnCuteMapLoaded;

	// Cute - fires on unload start
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnCuteMapLoaded OnCuteMapUnload;

	// Cute - fires when unload completes
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

	UFUNCTION(BlueprintCallable, Category = "LevelSaveFunctions")
	void SaveCollectable(int CollectableID);

	UFUNCTION(BlueprintCallable, Category = "LevelSaveFunctions")
	bool IsCollectablePickedUp(int CollectableID);

	UFUNCTION(BlueprintCallable, Category = "LevelSaveFunctions")
	TArray<bool> GetPickedCollectables();

protected:
	virtual void BeginPlay() override;
	void LoadlevelData();
	void ApplyInterfaceEvents(ESwapEvent event);

	UFUNCTION()
	void OnHorrorLevelLoaded();

	UFUNCTION()
	void OnCuteMapLevelLoaded();

	ULevelStreamingDynamic* streamedHorrorLevel;
	ULevelStreamingDynamic* streamedCuteLevel;
};