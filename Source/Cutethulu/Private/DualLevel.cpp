// Fill out your copyright notice in the Description page of Project Settings.


#include "DualLevel.h"
#include "SwappableInterface.h"
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
        unloadedAny = true;
    }

    if (streamedCuteLevel) {
        streamedCuteLevel->SetIsRequestingUnloadAndRemoval(true);
        streamedCuteLevel = nullptr;
        ApplyInterfaceEvents(ESwapEvent::CuteUnloaded);
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
        loadedState = (loadedState == ELoaded::CUTE) ? ELoaded::BOTH : ELoaded::HORROR;
        ApplyInterfaceEvents(ESwapEvent::HorrorLoaded);
        OnHorrorMapLoad.Broadcast();

        if (loadedState == ELoaded::BOTH)
            ApplyInterfaceEvents(ESwapEvent::BothLoaded);
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
        ApplyInterfaceEvents(ESwapEvent::HorrorUnloaded);
        OnHorrorMapUnload.Broadcast();
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
        if (loadedState == ELoaded::HORROR)
            loadedState = ELoaded::BOTH;
        else
            loadedState = ELoaded::CUTE;

        ApplyInterfaceEvents(ESwapEvent::CuteLoaded);
        OnCuteMapLoaded.Broadcast();
        if (loadedState == ELoaded::BOTH)
            ApplyInterfaceEvents(ESwapEvent::BothLoaded);
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
        ApplyInterfaceEvents(ESwapEvent::CuteUnloaded);
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

void ADualLevel::ApplyInterfaceEvents(ESwapEvent event)
{
    TArray<AActor*> swappableActors;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USwappableInterface::StaticClass(), swappableActors);
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