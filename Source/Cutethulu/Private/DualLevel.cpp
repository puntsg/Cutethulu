// Fill out your copyright notice in the Description page of Project Settings.


#include "DualLevel.h"
#include "SwappableInterface.h"
#include "CutethulhuSaveGame.h"
#include "Components/AudioComponent.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include <Engine/LevelStreamingDynamic.h>
#include <Kismet/GameplayStatics.h>


void ADualLevel::EnableTransitionLights()
{
    if (transitionDirectionalLight)
        transitionDirectionalLight->GetLightComponent()->SetIntensity(transitionLightIntensity);
    if (transitionSkyLight)
        transitionSkyLight->GetLightComponent()->SetIntensity(transitionLightIntensity);
}

void ADualLevel::DisableTransitionLights()
{
    if (transitionDirectionalLight)
        transitionDirectionalLight->GetLightComponent()->SetIntensity(0.f);
    if (transitionSkyLight)
        transitionSkyLight->GetLightComponent()->SetIntensity(0.f);
}


void ADualLevel::UnloadStreamedLevels()
{
    bool unloadedAny = false;

    if (streamedHorrorLevel) {
        streamedHorrorLevel->SetIsRequestingUnloadAndRemoval(true);
        streamedHorrorLevel = nullptr;
        ApplyInterfaceEvents(ESwapEvent::HorrorUnload);
        OnHorrorUnload.Broadcast();
        OnHorrorUnloaded.Broadcast();
        OnAnyUnload.Broadcast();
        OnAnyUnloaded.Broadcast();
        if (horrorAudioComponent != nullptr)
            horrorAudioComponent->FadeOut(audioFadeDuration, 0.f);
        unloadedAny = true;
    }

    if (streamedCuteLevel) {
        streamedCuteLevel->SetIsRequestingUnloadAndRemoval(true);
        streamedCuteLevel = nullptr;
        ApplyInterfaceEvents(ESwapEvent::CuteUnload);
        OnCuteUnload.Broadcast();
        OnCuteUnloaded.Broadcast();
        OnAnyUnload.Broadcast();
        OnAnyUnloaded.Broadcast();
        if (cuteAudioComponent != nullptr)
            cuteAudioComponent->FadeOut(audioFadeDuration, 0.f);
        unloadedAny = true;
    }

    if (unloadedAny) {
        loadedState = ELoaded::NONE;
        ApplyInterfaceEvents(ESwapEvent::BothUnloaded);
        OnBothUnload.Broadcast();
        OnBothUnloaded.Broadcast();
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
        ApplyInterfaceEvents(ESwapEvent::HorrorLoad);
        OnHorrorLoad.Broadcast();
        OnAnyLoad.Broadcast();
        horrorAudioComponent = UGameplayStatics::SpawnSound2D(this, horrorBgMusicClip);
        horrorAudioComponent->FadeIn(audioFadeDuration);
        streamedHorrorLevel->OnLevelShown.AddDynamic(this, &ADualLevel::OnHorrorMapLoadedFunc);
    }
}

void ADualLevel::OnHorrorMapLoadedFunc()
{
    loadedState = (loadedState == ELoaded::CUTE) ? ELoaded::BOTH : ELoaded::HORROR;

    DisableTransitionLights();

    ApplyInterfaceEvents(ESwapEvent::HorrorLoaded);
    OnHorrorLoaded.Broadcast();
    OnAnyLoaded.Broadcast();

    if (loadedState == ELoaded::BOTH) {
        ApplyInterfaceEvents(ESwapEvent::BothLoaded);
        OnBothLoaded.Broadcast();
    }
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

        ApplyInterfaceEvents(ESwapEvent::HorrorUnload);
        OnHorrorUnload.Broadcast();
        OnHorrorUnloaded.Broadcast();
        OnAnyUnload.Broadcast();
        OnAnyUnloaded.Broadcast();

        if (horrorAudioComponent != nullptr)
            horrorAudioComponent->FadeOut(audioFadeDuration, 0.f);
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
        ApplyInterfaceEvents(ESwapEvent::CuteLoad);
        OnCuteLoad.Broadcast();
        OnAnyLoad.Broadcast();
        cuteAudioComponent = UGameplayStatics::SpawnSound2D(this, cuteBgMusicClip);
        cuteAudioComponent->FadeIn(audioFadeDuration);
        streamedCuteLevel->OnLevelShown.AddDynamic(this, &ADualLevel::OnCuteMapLoadedFunc);
    }
}

void ADualLevel::OnCuteMapLoadedFunc()
{
    if (loadedState == ELoaded::HORROR)
        loadedState = ELoaded::BOTH;
    else
        loadedState = ELoaded::CUTE;

    DisableTransitionLights();

    ApplyInterfaceEvents(ESwapEvent::CuteLoaded);
    OnCuteLoaded.Broadcast();
    OnAnyLoaded.Broadcast();

    if (loadedState == ELoaded::BOTH) {
        ApplyInterfaceEvents(ESwapEvent::BothLoaded);
        OnBothLoaded.Broadcast();
    }
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

        ApplyInterfaceEvents(ESwapEvent::CuteUnload);
        OnCuteUnload.Broadcast();
        OnCuteUnloaded.Broadcast();
        OnAnyUnload.Broadcast();
        OnAnyUnloaded.Broadcast();

        if (cuteAudioComponent != nullptr)
            cuteAudioComponent->FadeOut(audioFadeDuration, 0.f);
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

    EnableTransitionLights();

    GetWorldTimerManager().ClearTimer(AudioSwapTimerHandle);

    ApplyInterfaceEvents(ESwapEvent::Swap);
    OnSwap.Broadcast();

    if (loadedState == ELoaded::CUTE) {
        UnloadCuteLevel();
        GetWorldTimerManager().SetTimer(
            AudioSwapTimerHandle,
            this,
            &ADualLevel::DelayedLoadHorrorLevel,
            audioFadeDuration,
            false
        );
    }
    else {
        UnloadHorrorLevel();
        GetWorldTimerManager().SetTimer(
            AudioSwapTimerHandle,
            this,
            &ADualLevel::DelayedLoadCuteLevel,
            audioFadeDuration,
            false
        );
    }

    ApplyInterfaceEvents(ESwapEvent::Swapped);
    OnSwapped.Broadcast();
}

void ADualLevel::DelayedLoadHorrorLevel()
{
    LoadHorrorLevel();
}

void ADualLevel::DelayedLoadCuteLevel()
{
    LoadCuteLevel();
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

    DisableTransitionLights();

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
        case ESwapEvent::AnyLoad:
            ISwappableInterface::Execute_OnAnyLoad(currentSwappableActor);
            break;
        case ESwapEvent::AnyLoaded:
            ISwappableInterface::Execute_OnAnyLoaded(currentSwappableActor);
            break;
        case ESwapEvent::AnyUnload:
            ISwappableInterface::Execute_OnAnyUnload(currentSwappableActor);
            break;
        case ESwapEvent::AnyUnloaded:
            ISwappableInterface::Execute_OnAnyUnloaded(currentSwappableActor);
            break;
        case ESwapEvent::HorrorLoad:
            ISwappableInterface::Execute_OnHorrorLoad(currentSwappableActor);
            break;
        case ESwapEvent::HorrorLoaded:
            ISwappableInterface::Execute_OnHorrorLoaded(currentSwappableActor);
            break;
        case ESwapEvent::HorrorUnload:
            ISwappableInterface::Execute_OnHorrorUnload(currentSwappableActor);
            break;
        case ESwapEvent::HorrorUnloaded:
            ISwappableInterface::Execute_OnHorrorUnloaded(currentSwappableActor);
            break;
        case ESwapEvent::CuteLoad:
            ISwappableInterface::Execute_OnCuteLoad(currentSwappableActor);
            break;
        case ESwapEvent::CuteLoaded:
            ISwappableInterface::Execute_OnCuteLoaded(currentSwappableActor);
            break;
        case ESwapEvent::CuteUnload:
            ISwappableInterface::Execute_OnCuteUnload(currentSwappableActor);
            break;
        case ESwapEvent::CuteUnloaded:
            ISwappableInterface::Execute_OnCuteUnloaded(currentSwappableActor);
            break;
        case ESwapEvent::BothLoad:
            ISwappableInterface::Execute_OnBothLoad(currentSwappableActor);
            break;
        case ESwapEvent::BothLoaded:
            ISwappableInterface::Execute_OnBothLoaded(currentSwappableActor);
            break;
        case ESwapEvent::BothUnload:
            ISwappableInterface::Execute_OnBothUnload(currentSwappableActor);
            break;
        case ESwapEvent::BothUnloaded:
            ISwappableInterface::Execute_OnBothUnloaded(currentSwappableActor);
            break;
        case ESwapEvent::Swap:
            ISwappableInterface::Execute_OnSwap(currentSwappableActor);
            break;
        case ESwapEvent::Swapped:
            ISwappableInterface::Execute_OnSwapped(currentSwappableActor);
            break;
        }
    }
}