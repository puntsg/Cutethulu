// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DualLevel.h"
#include "DualLevelFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CUTETHULU_API UDualLevelFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "DualLevel", meta = (WorldContext = "WorldContextObject"))
	static ADualLevel* GetDualLevel(const UObject* WorldContextObject);
};
