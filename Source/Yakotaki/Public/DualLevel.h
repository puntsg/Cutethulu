// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/LevelStreamingDynamic.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"
#include "Blueprint/UserWidget.h"
#include "DualLevel.generated.h"

UENUM(BlueprintType)
enum class ELoaded : uint8 {
	NONE	UMETA(DisplayName = "None"),
	CUTE UMETA(DisplayName = "Cute"),
	HORROR UMETA(DisplayName = "Horror"),
	BOTH UMETA(DisplayName = "Both")
};

UENUM(BlueprintType)
enum class EStreamingState : uint8 {
	NORMAL	UMETA(DisplayName = "Normal"),
	LOADING UMETA(DisplayName = "Loading"),
	UNLOADING UMETA(DisplayName = "Unloading"),
	SWAPPING UMETA(DisplayName = "Swapping")
};

UENUM()
enum class ESwapEvent : uint8 {
	//Any
	AnyLoad,
	AnyLoaded,
	AnyUnload,
	AnyUnloaded,
	//Horror events
	HorrorLoad,
	HorrorLoaded,
	HorrorUnload,
	HorrorUnloaded,
	//CuteEvents
	CuteLoad,
	CuteLoaded,
	CuteUnload,
	CuteUnloaded,
	//BothEvents
	BothLoad,
	BothLoaded,
	BothUnload,
	BothUnloaded,
	//Swap
	Swap,
	Swapped
};

#pragma region EventsDefinition
//Any
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyLoad);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyUnload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyUnloaded);
//Horror Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHorrorLoad);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHorrorLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHorrorUnload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHorrorUnloaded);
//Cute events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCuteLoad);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCuteLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCuteUnload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCuteUnloaded);
//Both Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBothLoad);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBothLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBothUnload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBothUnloaded);
//Swap
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwap);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwapped);

//InitialSequence
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitialSequenceComplete);
#pragma endregion

UCLASS()
class YAKOTAKI_API ADualLevel : public ALevelScriptActor
{
	GENERATED_BODY()

public:

	#pragma region Map Data
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int LevelID;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText LevelName;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int numOfCollectables;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<APlayerStart> DefaultPlayerStart;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<APlayerStart> LevelCompletedPlayerStart;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ToolTip = "Borra los datos del nivel"))
	bool deleteLevelSaveData;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<AActor> initialLookatActor;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ALevelSequenceActor> initialSequence;
	#pragma endregion

	#pragma region Streaming Config
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DualLevel|Streaming", meta = (ToolTip = "Si al cargar el nivel principal se quiere que se aplique el estado definido en loadedState  (el valor de abajo creo)"))
	bool overrideLoadedState;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DualLevel|Streaming", meta = (ToolTip = "Valor de que esta cargado"))
	ELoaded loadedState;
	UPROPERTY(EditAnywhere, Category = "DualLevel|Streaming|Horror")
	TSoftObjectPtr<UWorld> horrorLevel;
	UPROPERTY(EditAnywhere, Category = "DualLevel|Streaming|Cute")
	TSoftObjectPtr<UWorld> cuteLevel;
	UPROPERTY(EditAnywhere, Category = "DualLevel|Streaming")
	TSubclassOf<class UUserWidget> loadingScreenClass = LoadClass<UUserWidget>(nullptr,TEXT("/Game/Project/00_Generic/Blueprints/UI_HUD/WBP_LoadingScreen.WBP_LoadingScreen_C"));
	UPROPERTY(EditAnywhere, Category = "DualLevel|Streaming")
	TSoftObjectPtr<UUserWidget> loadingScreen;
	UPROPERTY(EditAnywhere, Category = "DualLevel|Streaming")
	USoundBase* mapSwappingSoundEffect;
	#pragma endregion

	#pragma region Audio
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DualLevel|Streaming|Audio")
	UFMODEvent* musicEvent = LoadObject<UFMODEvent>(nullptr, TEXT("FMODEvent'/Game/FMOD/Events/Play_OneShot.Play_OneShot'"));
	FFMODEventInstance musicEventInstance;
	UPROPERTY(EditAnywhere, Category = "DualLevel|Streaming")
	float audioFadeDuration = 1.f;
	#pragma endregion

	#pragma region Lighting
	UPROPERTY(EditAnywhere, Category = "DualLevel|Lighting(Not in use)")
	TObjectPtr<ADirectionalLight> transitionDirectionalLight;
	UPROPERTY(EditAnywhere, Category = "DualLevel|Lighting(Not in use)")
	TObjectPtr<ASkyLight> transitionSkyLight;
	UPROPERTY(EditAnywhere, Category = "DualLevel|Lighting(Not in use)")
	float transitionLightIntensity = 5.f;
	#pragma endregion

	#pragma region Events
	///Any
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnAnyLoad OnAnyLoad;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnAnyLoaded OnAnyLoaded;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnAnyUnload OnAnyUnload;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnAnyUnloaded OnAnyUnloaded;
	///Horror
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnHorrorLoad OnHorrorLoad;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnHorrorLoaded OnHorrorLoaded;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnHorrorUnload OnHorrorUnload;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnHorrorUnloaded OnHorrorUnloaded;
	///Cute
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnCuteLoad OnCuteLoad;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnCuteLoaded OnCuteLoaded;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnCuteUnload OnCuteUnload;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnCuteUnloaded OnCuteUnloaded;
	///Both
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnBothLoad OnBothLoad;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnBothLoaded OnBothLoaded;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnBothUnload OnBothUnload;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnBothUnloaded OnBothUnloaded;
	///Swap
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnSwap OnSwap;
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnSwapped OnSwapped;
	///InitialSequence
	UPROPERTY(BlueprintAssignable, Category = "LevelLoadEvents")
	FOnInitialSequenceComplete OnInitialSequenceComplete;
	#pragma endregion

	#pragma region Level Streaming Functions
	UFUNCTION()
	void NotifySequenceEnd();
	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void LoadHorrorLevel();
	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void UnloadHorrorLevel();
	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void LoadCuteLevel();
	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void UnloadCuteLevel();
	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void UnloadStreamedLevels();
	UFUNCTION(BlueprintCallable, Category = "LevelLoadFunctions")
	void SwapLevel();
	#pragma endregion

	#pragma region Save Functions
	UFUNCTION(BlueprintCallable, Category = "LevelSaveFunctions")
	void SaveCollectable(int CollectableID);
	UFUNCTION(BlueprintCallable, Category = "LevelSaveFunctions")
	bool IsLevelCompleted();
	UFUNCTION(BlueprintCallable, Category = "LevelSaveFunctions")
	bool IsCollectablePickedUp(int CollectableID);
	UFUNCTION(BlueprintCallable, Category = "LevelSaveFunctions")
	TArray<bool> GetPickedCollectables();
	#pragma endregion

protected:

	#pragma region Lifecycle
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void LoadlevelData();
	#pragma endregion

	#pragma region Level Callbacks
	UFUNCTION()
	void OnHorrorMapLoadedFunc();
	UFUNCTION()
	void OnCuteMapLoadedFunc();
	#pragma endregion

	#pragma region Lighting Internals
	void EnableTransitionLights();
	void DisableTransitionLights();
	void GetLoadedLevelLights(ULevelStreamingDynamic* streamedLevel);
	void InterpLights();
	TObjectPtr<UDirectionalLightComponent> SublevelDirectionalLight;
	TObjectPtr<USkyLightComponent> SublevelSkyLight;
	FTimerHandle LightsInterpolationTimer;
	#pragma endregion

	#pragma region Interface Events
	void ApplyInterfaceEvents(ESwapEvent event);
	#pragma endregion

	#pragma region State
	ULevelStreamingDynamic* streamedHorrorLevel;
	ULevelStreamingDynamic* streamedCuteLevel;
	FTimerHandle AudioSwapTimerHandle;
	#pragma endregion
};
