// Fill out your copyright notice in the Description page of Project Settings.


#include "DualLevelFunctionLibrary.h"
#include "Engine/World.h"

ADualLevel* UDualLevelFunctionLibrary::GetDualLevel(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
		return nullptr;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
		return nullptr;
	return Cast<ADualLevel>(World->GetLevelScriptActor());
}
