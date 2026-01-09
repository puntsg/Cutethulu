// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "DualLevel.generated.h"



UENUM(BlueprintType)
enum class EMapType : uint8 {
	Single	UMETA(DisplayName = "None"),
	ONCE UMETA(DisplayName = "Once"),
	LOOP UMETA(DisplayName = "Loop"),
	PINGPONG UMETA(DisplayName = "PingPong")
};

UCLASS()
class CUTETHULU_API ADualLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Streaming")
	TSoftObjectPtr<UWorld> corruptedLevel;
	UPROPERTY(EditAnywhere, Category = "Streaming")
	TSoftObjectPtr<UWorld> healedAsset;+


protected:
	virtual void BeginPlay() override;

};
