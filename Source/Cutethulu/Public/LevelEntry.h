// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LevelEntry.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CUTETHULU_API FLevelEntry : public FTableRowBase
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 LevelID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText LevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UWorld> LevelToLoad;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText HiddenName = FText::FromString("???");

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* levelThumbnail;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* completedLevelThumbnail;
};
