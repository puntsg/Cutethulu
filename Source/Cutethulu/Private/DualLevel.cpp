// Fill out your copyright notice in the Description page of Project Settings.


#include "DualLevel.h"
#include <Engine/LevelStreamingDynamic.h>
#include <Kismet/GameplayStatics.h>

void ADualLevel::UnloadStreamedLevels()
{
}

void ADualLevel::LoadHorrorLevel()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LoadingHorrorLevel"));
    bool bSuccess = false;
    ULevelStreamingDynamic* Stream = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        this,
        corruptedLevel,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        bSuccess
    );
    OnHorrorMapLoad.Broadcast();
}

void ADualLevel::UnloadHorrorLevel()
{
}

void ADualLevel::LoadCuteLevel()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LoadingCuteLevel"));
    bool bSuccess = false;
    ULevelStreamingDynamic* Stream = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        this,
        healedLevel,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        bSuccess
    );
    OnCuteMapLoaded.Broadcast();
}

void ADualLevel::UnloadCuteLevel()
{
}

void ADualLevel::SwapLevel()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hi, i'm the level class swapping"));
}

void ADualLevel::BeginPlay() {
    Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hi, i'm the level class"));
}