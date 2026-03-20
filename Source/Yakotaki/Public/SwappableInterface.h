#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SwappableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USwappableInterface : public UInterface
{
	GENERATED_BODY()
};

class YAKOTAKI_API ISwappableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//ANY
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnAnyLoad();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnAnyLoaded();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnAnyUnload();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnAnyUnloaded();

	//Horror events
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnHorrorLoad();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnHorrorLoaded();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnHorrorUnload();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnHorrorUnloaded();

	//Cute events
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnCuteLoad();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnCuteLoaded();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnCuteUnload();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnCuteUnloaded();

	//Both Events
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnBothLoad();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnBothLoaded();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnBothUnload();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnBothUnloaded();

	//Swap
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnSwap();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnSwapped();
};
