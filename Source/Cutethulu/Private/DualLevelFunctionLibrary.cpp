// Fill out your copyright notice in the Description page of Project Settings.


#include "DualLevelFunctionLibrary.h"
#include "Engine/World.h"

ADualLevel* UDualLevelFunctionLibrary::GetDualLevel(const UObject* WorldContextObject)
{
	if (!WorldContextObject) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("No WorldContextObject assigned/found"));
		return nullptr;
	}
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("World not found or assigned"));
		return nullptr;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Returning ADualLevel"));
	//return Cast<ADualLevel>(World->GetLevelScriptActor());
	for (ULevel* Level : World->GetLevels()) {
		if (!Level)
			continue;
		ADualLevel* DualLevel = Cast<ADualLevel>(Level->GetLevelScriptActor());
		if (DualLevel)
			return DualLevel;
	}
	return nullptr;
}
