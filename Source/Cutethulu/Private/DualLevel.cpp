// Fill out your copyright notice in the Description page of Project Settings.


#include "DualLevel.h"
#include <Engine/LevelStreamingDynamic.h>
#include <Kismet/GameplayStatics.h>

void ADualLevel::UnloadStreamedLevels()
{
}

void ADualLevel::LoadHorrorLevel()
{
}

void ADualLevel::UnloadHorrorLevel()
{
}

void ADualLevel::LoadCuteLevel()
{
}

void ADualLevel::UnloadCuteLevel()
{
}

void ADualLevel::SwapLevel()
{
}

void ADualLevel::BeginPlay() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hi, i'm the level class"));
    bool bSuccess = false;
    ULevelStreamingDynamic* Stream = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        this,
        corruptedLevel,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        bSuccess
    );
    
    this->OnLevelLoaded.Broadcast();
	
}