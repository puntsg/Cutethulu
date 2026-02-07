// Fill out your copyright notice in the Description page of Project Settings.


#include "DualLevel.h"
#include "SwappableInterface.h"
#include "CutethulhuSaveGame.h"
#include <Engine/LevelStreamingDynamic.h>
#include <Kismet/GameplayStatics.h>

void ADualLevel::UnloadStreamedLevels()
{
    bool unloadedAny = false;

    if (streamedHorrorLevel) {
        streamedHorrorLevel->SetIsRequestingUnloadAndRemoval(true);
        streamedHorrorLevel = nullptr;
        ApplyInterfaceEvents(ESwapEvent::HorrorUnloaded);
        OnHorrorMapUnload.Broadcast();
        OnHorrorMapUnloaded.Broadcast();
        unloadedAny = true;
    }

    if (streamedCuteLevel) {
        streamedCuteLevel->SetIsRequestingUnloadAndRemoval(true);
        streamedCuteLevel = nullptr;
        ApplyInterfaceEvents(ESwapEvent::CuteUnloaded);
        OnCuteMapUnload.Broadcast();
        OnCuteMapUnloaded.Broadcast();
        unloadedAny = true;
    }

    if (unloadedAny) {
        loadedState = ELoaded::NONE;
        ApplyInterfaceEvents(ESwapEvent::BothUnloaded);
    }
    else
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No levels were loaded"));
}

void ADualLevel::LoadHorrorLevel()
{
    if (streamedHorrorLevel)
        return;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LoadingHorrorLevel"));
    bool bSuccess = false;
    streamedHorrorLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        this,
        horrorLevel,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        bSuccess
    );

    if (bSuccess) {
        OnHorrorMapLoad.Broadcast();
        streamedHorrorLevel->OnLevelShown.AddDynamic(this, &ADualLevel::OnHorrorLevelLoaded);
    }
}

void ADualLevel::OnHorrorLevelLoaded()
{
    loadedState = (loadedState == ELoaded::CUTE) ? ELoaded::BOTH : ELoaded::HORROR;
    ApplyInterfaceEvents(ESwapEvent::HorrorLoaded);
    OnHorrorMapLoaded.Broadcast();

    if (loadedState == ELoaded::BOTH)
        ApplyInterfaceEvents(ESwapEvent::BothLoaded);
}

void ADualLevel::UnloadHorrorLevel()
{
    if (streamedHorrorLevel) {
        streamedHorrorLevel->SetIsRequestingUnloadAndRemoval(true);
        streamedHorrorLevel = nullptr;
        if (loadedState == ELoaded::BOTH)
            loadedState = ELoaded::CUTE;
        else
            loadedState = ELoaded::NONE;
        ApplyInterfaceEvents(ESwapEvent::HorrorUnloaded);
        OnHorrorMapUnload.Broadcast();
        OnHorrorMapUnloaded.Broadcast();
    }
    else
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Horror level was not loaded"));
}

void ADualLevel::LoadCuteLevel()
{
    if (streamedCuteLevel)
        return;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LoadingCuteLevel"));
    bool bSuccess = false;
    streamedCuteLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        this,
        cuteLevel,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        bSuccess
    );

    if (bSuccess) {
        OnCuteMapLoad.Broadcast();
        streamedCuteLevel->OnLevelShown.AddDynamic(this, &ADualLevel::OnCuteMapLevelLoaded);
    }
}

void ADualLevel::OnCuteMapLevelLoaded()
{
    if (loadedState == ELoaded::HORROR)
        loadedState = ELoaded::BOTH;
    else
        loadedState = ELoaded::CUTE;

    ApplyInterfaceEvents(ESwapEvent::CuteLoaded);
    OnCuteMapLoaded.Broadcast();

    if (loadedState == ELoaded::BOTH)
        ApplyInterfaceEvents(ESwapEvent::BothLoaded);
}

void ADualLevel::UnloadCuteLevel()
{
    if (streamedCuteLevel) {
        streamedCuteLevel->SetIsRequestingUnloadAndRemoval(true);
        streamedCuteLevel = nullptr;
        if (loadedState == ELoaded::BOTH)
            loadedState = ELoaded::HORROR;
        else
            loadedState = ELoaded::NONE;
        ApplyInterfaceEvents(ESwapEvent::CuteUnloaded);
        OnCuteMapUnload.Broadcast();
        OnCuteMapUnloaded.Broadcast();
    }
    else
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cute level was not loaded"));
}

void ADualLevel::SwapLevel()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hi, i'm the level class swapping"));
    if (loadedState == ELoaded::BOTH || loadedState == ELoaded::NONE) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No streamedLevels Loaded or both loaded"));
        return;
    }
    else {
        if (loadedState == ELoaded::CUTE) {
            UnloadCuteLevel();
            LoadHorrorLevel();
        }
        else {
            UnloadHorrorLevel();
            LoadCuteLevel();
        }
        ApplyInterfaceEvents(ESwapEvent::Swapped);
        OnMapChange.Broadcast();
    }
}

void ADualLevel::SaveCollectable(int CollectableID)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Start saving"));
    UCutethulhuSaveGame* SaveGameInstance = nullptr;
    if (UGameplayStatics::DoesSaveGameExist("player", 0))
        SaveGameInstance = Cast<UCutethulhuSaveGame>(UGameplayStatics::LoadGameFromSlot("player", 0));

    if (!SaveGameInstance)
        SaveGameInstance = Cast<UCutethulhuSaveGame>(UGameplayStatics::CreateSaveGameObject(UCutethulhuSaveGame::StaticClass()));

    while (SaveGameInstance->LevelsData.Num() <= this->LevelID)
    {
        FLevelData newLevelData;
        newLevelData.LevelName = this->LevelName;
        newLevelData.completed = false;
        SaveGameInstance->LevelsData.Add(newLevelData);
    }
    FLevelData& currentLevelData = SaveGameInstance->LevelsData[this->LevelID];

    while (currentLevelData.pickedCollectables.Num() <= CollectableID)
        currentLevelData.pickedCollectables.Add(false);

    currentLevelData.pickedCollectables[CollectableID] = true;
    SaveGameInstance->SaveGame();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Game saved"));
}

bool ADualLevel::IsCollectablePickedUp(int CollectableID)
{
    if (!UGameplayStatics::DoesSaveGameExist("player", 0)) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Game save doesn't exist"));
        return false;
    }

    UCutethulhuSaveGame* SaveGameInstance = Cast<UCutethulhuSaveGame>(UGameplayStatics::LoadGameFromSlot("player", 0));

    if (!SaveGameInstance) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("SaveGameInstance is null"));
        return false;
    }

    if (this->LevelID >= SaveGameInstance->LevelsData.Num()) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Game save doesn't contain any level"));
        return false;
    }

    const FLevelData& levelData = SaveGameInstance->LevelsData[this->LevelID];
    if (CollectableID >= levelData.pickedCollectables.Num()) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Current level doesn't contain collectable data"));
        return false;
    }

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Checking collectableData"));
    return levelData.pickedCollectables[CollectableID];
}

TArray<bool> ADualLevel::GetPickedCollectables()
{
    TArray<bool> defaultArray;
    defaultArray.Init(false, this->numOfCollectables);

    if (!UGameplayStatics::DoesSaveGameExist("player", 0)) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Game save doesn't exist"));
        return defaultArray;
    }

    UCutethulhuSaveGame* SaveGameInstance = Cast<UCutethulhuSaveGame>(UGameplayStatics::LoadGameFromSlot("player", 0));

    if (!SaveGameInstance) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("SaveGameInstance is null"));
        return defaultArray;
    }

    if (this->LevelID >= SaveGameInstance->LevelsData.Num()) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Game save doesn't contain any level"));
        return defaultArray;
    }

    const FLevelData& levelData = SaveGameInstance->LevelsData[this->LevelID];
    if (0 >= levelData.pickedCollectables.Num()) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Current level doesn't contain collectable data"));
        return defaultArray;
    }
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Checking collectableData"));
    return levelData.pickedCollectables;
}

void ADualLevel::BeginPlay() {
    Super::BeginPlay();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hi, i'm the level class"));
    if (overrideLoadedState) {
        if (loadedState == ELoaded::CUTE || loadedState == ELoaded::BOTH)
            LoadCuteLevel();
        if (loadedState == ELoaded::HORROR || loadedState == ELoaded::BOTH)
            LoadHorrorLevel();
    }
}

void ADualLevel::LoadlevelData()
{
    if (UGameplayStatics::DoesSaveGameExist("player", 0)) {
        UCutethulhuSaveGame* currentSaveGame = Cast<UCutethulhuSaveGame>(UGameplayStatics::LoadGameFromSlot("player", 0));
        FLevelData currentLevelData = currentSaveGame->LevelsData[LevelID];
    }
}

void ADualLevel::ApplyInterfaceEvents(ESwapEvent event)
{
    TArray<AActor*> swappableActors;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USwappableInterface::StaticClass(), swappableActors);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Finding Actors With Swappable interface"));
    for (AActor* currentSwappableActor : swappableActors) {
        switch (event)
        {
        case ESwapEvent::HorrorLoaded:
            ISwappableInterface::Execute_OnHorrorLoaded(currentSwappableActor);
            break;
        case ESwapEvent::HorrorUnloaded:
            ISwappableInterface::Execute_OnHorrorUnloaded(currentSwappableActor);
            break;
        case ESwapEvent::CuteLoaded:
            ISwappableInterface::Execute_OnCuteLoaded(currentSwappableActor);
            break;
        case ESwapEvent::CuteUnloaded:
            ISwappableInterface::Execute_OnCuteUnloaded(currentSwappableActor);
            break;
        case ESwapEvent::BothLoaded:
            ISwappableInterface::Execute_OnBothLoaded(currentSwappableActor);
            break;
        case ESwapEvent::BothUnloaded:
            ISwappableInterface::Execute_OnBothUnloaded(currentSwappableActor);
            break;
        case ESwapEvent::Swapped:
            ISwappableInterface::Execute_OnSwap(currentSwappableActor);
            break;
        }
    }
}