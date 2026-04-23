// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TutorialEntry.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct YAKOTAKI_API FTutorialEntry : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString tutorialName;
};