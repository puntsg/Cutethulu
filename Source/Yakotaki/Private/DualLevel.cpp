// Fill out your copyright notice in the Description page of Project Settings.


#include "DualLevel.h"
#include "SwappableInterface.h"
#include "YakotakiSaveGame.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "GameFramework/GameModeBase.h"
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
        if (horrorBgMusicClip) {
            horrorAudioComponent = UGameplayStatics::SpawnSound2D(this, horrorBgMusicClip);
            horrorAudioComponent->FadeIn(audioFadeDuration);
        }
        streamedHorrorLevel->OnLevelShown.AddDynamic(this, &ADualLevel::OnHorrorMapLoadedFunc);
    }
}

void ADualLevel::OnHorrorMapLoadedFunc()
{
    loadedState = (loadedState == ELoaded::CUTE) ? ELoaded::BOTH : ELoaded::HORROR;

    GetLoadedLevelLights(streamedHorrorLevel);
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

        loadedState = (loadedState == ELoaded::BOTH) ? ELoaded::CUTE : ELoaded::NONE;

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
        if (cuteBgMusicClip) {
            cuteAudioComponent = UGameplayStatics::SpawnSound2D(this, cuteBgMusicClip);
            cuteAudioComponent->FadeIn(audioFadeDuration);
        }
        streamedCuteLevel->OnLevelShown.AddDynamic(this, &ADualLevel::OnCuteMapLoadedFunc);
    }
}

void ADualLevel::OnCuteMapLoadedFunc()
{
    loadedState = (loadedState == ELoaded::HORROR) ? ELoaded::BOTH : loadedState = ELoaded::CUTE;

    GetLoadedLevelLights(streamedCuteLevel);
    DisableTransitionLights();

    ApplyInterfaceEvents(ESwapEvent::CuteLoaded);
    OnCuteLoaded.Broadcast();
    OnAnyLoaded.Broadcast();

    if (loadedState == ELoaded::BOTH) {
        ApplyInterfaceEvents(ESwapEvent::BothLoaded);
        OnBothLoaded.Broadcast();
    }
}

void ADualLevel::GetLoadedLevelLights(ULevelStreamingDynamic* streamedLevel)
{
    SublevelSkyLight = nullptr;
    SublevelDirectionalLight = nullptr;

    if (!streamedLevel) return;
    ULevel* LoadedLevel = streamedLevel->GetLoadedLevel();
    if (!LoadedLevel) return;

    for (AActor* Actor : LoadedLevel->Actors)
    {
        if (!Actor) continue;
        if (!SublevelDirectionalLight)
            if (ADirectionalLight* DirLight = Cast<ADirectionalLight>(Actor))
                SublevelDirectionalLight = Cast<UDirectionalLightComponent>(DirLight->GetLightComponent());
        if (!SublevelSkyLight)
            if (ASkyLight* SkyLight = Cast<ASkyLight>(Actor))
                SublevelSkyLight = Cast<USkyLightComponent>(SkyLight->GetLightComponent());
        if (SublevelDirectionalLight && SublevelSkyLight)
            break;
    }

    
    GetWorldTimerManager().ClearTimer(LightsInterpolationTimer);
    GetWorldTimerManager().SetTimer(LightsInterpolationTimer, this, &ADualLevel::InterpLights, 0.016f, true);
}

void ADualLevel::InterpLights()
{
    if (!GetWorld()) return;
    float DeltaTime = GetWorld()->GetDeltaSeconds();
    bool bDirDone = true;
    bool bSkyDone = true;


    if (bDirDone && bSkyDone)
        GetWorldTimerManager().ClearTimer(LightsInterpolationTimer);
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
        LoadHorrorLevel();
    }
    else {
        UnloadHorrorLevel();
        LoadCuteLevel();
    }

    ApplyInterfaceEvents(ESwapEvent::Swapped);
    OnSwapped.Broadcast();
}

void ADualLevel::SaveCollectable(int CollectableID)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Start saving"));
    UYakotakiSaveGame* SaveGameInstance = nullptr;
    if (UGameplayStatics::DoesSaveGameExist("player", 0))
        SaveGameInstance = Cast<UYakotakiSaveGame>(UGameplayStatics::LoadGameFromSlot("player", 0));

    if (!SaveGameInstance)
        SaveGameInstance = Cast<UYakotakiSaveGame>(UGameplayStatics::CreateSaveGameObject(UYakotakiSaveGame::StaticClass()));

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



bool ADualLevel::IsLevelCompleted()
{
    bool bHasLevelBeenCompleted = false;
    if (UGameplayStatics::DoesSaveGameExist("player", 0)) {
        UYakotakiSaveGame* SaveGameInstance = Cast<UYakotakiSaveGame>(UGameplayStatics::LoadGameFromSlot("player", 0));
        if (SaveGameInstance)
            bHasLevelBeenCompleted = SaveGameInstance->GetIfLevelCompleted(this->LevelID);
    }
    return bHasLevelBeenCompleted;
}

bool ADualLevel::IsCollectablePickedUp(int CollectableID)
{
    if (!UGameplayStatics::DoesSaveGameExist("player", 0)) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Game save doesn't exist"));
        return false;
    }

    UYakotakiSaveGame* SaveGameInstance = Cast<UYakotakiSaveGame>(UGameplayStatics::LoadGameFromSlot("player", 0));

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

    UYakotakiSaveGame* SaveGameInstance = Cast<UYakotakiSaveGame>(UGameplayStatics::LoadGameFromSlot("player", 0));

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


void ADualLevel::BeginPlay()
{
    Super::BeginPlay();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hi, i'm the level class"));
    DisableTransitionLights();

    if (deleteLevelSaveData) {
        if (UGameplayStatics::DoesSaveGameExist("player", 0)) {
            UYakotakiSaveGame* SaveGameInstance = Cast<UYakotakiSaveGame>(
                UGameplayStatics::LoadGameFromSlot("player", 0));
            if (SaveGameInstance)
                SaveGameInstance->DeleteLevelData(LevelID);
        }
    }

    if (overrideLoadedState) {
        if (loadedState == ELoaded::CUTE || loadedState == ELoaded::BOTH)
            LoadCuteLevel();
        if (loadedState == ELoaded::HORROR || loadedState == ELoaded::BOTH)
            LoadHorrorLevel();
    }
    else {
        if (IsLevelCompleted())
            LoadCuteLevel();
        else
            LoadHorrorLevel();
    }

    if (DefaultPlayerStart) {
        APlayerStart* TargetStart = DefaultPlayerStart;
        if (IsLevelCompleted() && LevelCompletedPlayerStart)
            TargetStart = LevelCompletedPlayerStart;

        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
        if (PC && GameMode) {
            PC->GetPawn()->SetActorLocationAndRotation(
                TargetStart->GetActorLocation(),
                TargetStart->GetActorRotation()
            );
            PC->SetControlRotation(TargetStart->GetActorRotation());
        }
    }
}

void ADualLevel::LoadlevelData()
{
    if (UGameplayStatics::DoesSaveGameExist("player", 0)) {
        UYakotakiSaveGame* currentSaveGame = Cast<UYakotakiSaveGame>(UGameplayStatics::LoadGameFromSlot("player", 0));
        FLevelData currentLevelData = currentSaveGame->LevelsData[LevelID];
    }
}

void ADualLevel::ApplyInterfaceEvents(ESwapEvent event)
{
    TArray<UObject*> swappableObjects;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Finding Actors With Swappable interface"));
    TArray<AActor*> swappableActors;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USwappableInterface::StaticClass(), swappableActors);

    for (AActor* currentSwappableActor : swappableActors) {
        swappableObjects.Add(currentSwappableActor);
        TArray<UActorComponent*> components;
        currentSwappableActor->GetComponents(components);
        for (UActorComponent* comp : components)
            if (comp->Implements<USwappableInterface>())
                swappableObjects.Add(comp);

    }
    for (UObject* currentSwappableObject : swappableObjects) {
        switch (event)
        {
        case ESwapEvent::AnyLoad:
            ISwappableInterface::Execute_OnAnyLoad(currentSwappableObject);
            break;
        case ESwapEvent::AnyLoaded:
            ISwappableInterface::Execute_OnAnyLoaded(currentSwappableObject);
            break;
        case ESwapEvent::AnyUnload:
            ISwappableInterface::Execute_OnAnyUnload(currentSwappableObject);
            break;
        case ESwapEvent::AnyUnloaded:
            ISwappableInterface::Execute_OnAnyUnloaded(currentSwappableObject);
            break;
        case ESwapEvent::HorrorLoad:
            ISwappableInterface::Execute_OnHorrorLoad(currentSwappableObject);
            break;
        case ESwapEvent::HorrorLoaded:
            ISwappableInterface::Execute_OnHorrorLoaded(currentSwappableObject);
            break;
        case ESwapEvent::HorrorUnload:
            ISwappableInterface::Execute_OnHorrorUnload(currentSwappableObject);
            break;
        case ESwapEvent::HorrorUnloaded:
            ISwappableInterface::Execute_OnHorrorUnloaded(currentSwappableObject);
            break;
        case ESwapEvent::CuteLoad:
            ISwappableInterface::Execute_OnCuteLoad(currentSwappableObject);
            break;
        case ESwapEvent::CuteLoaded:
            ISwappableInterface::Execute_OnCuteLoaded(currentSwappableObject);
            break;
        case ESwapEvent::CuteUnload:
            ISwappableInterface::Execute_OnCuteUnload(currentSwappableObject);
            break;
        case ESwapEvent::CuteUnloaded:
            ISwappableInterface::Execute_OnCuteUnloaded(currentSwappableObject);
            break;
        case ESwapEvent::BothLoad:
            ISwappableInterface::Execute_OnBothLoad(currentSwappableObject);
            break;
        case ESwapEvent::BothLoaded:
            ISwappableInterface::Execute_OnBothLoaded(currentSwappableObject);
            break;
        case ESwapEvent::BothUnload:
            ISwappableInterface::Execute_OnBothUnload(currentSwappableObject);
            break;
        case ESwapEvent::BothUnloaded:
            ISwappableInterface::Execute_OnBothUnloaded(currentSwappableObject);
            break;
        case ESwapEvent::Swap:
            ISwappableInterface::Execute_OnSwap(currentSwappableObject);
            break;
        case ESwapEvent::Swapped:
            ISwappableInterface::Execute_OnSwapped(currentSwappableObject);
            break;
        }
    }
}