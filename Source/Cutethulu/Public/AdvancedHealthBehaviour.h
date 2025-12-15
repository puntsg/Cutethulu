// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AdvancedHealthBehaviour.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLifeConsumed, int, remainingLifes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllLifesConsumed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthAlter,float,health,float,maxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthAltered,float,health,float,maxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamage,float,health,float,maxHealth,float,damageValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamaged,float,health,float,maxHealth,float,damageValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHeal,float,health,float,maxHealth,float,healingValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealed,float,health,float,maxHealth,float,healingValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class CUTETHULU_API UAdvancedHealthBehaviour : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	//Event Dispatchers

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnDie OnDie;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnLifeConsumed OnLifeConsumed;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnAllLifesConsumed OnAllLifesConsumed;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnHealthAlter OnHealthAlter;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnHealthAltered OnHealthAltered;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnDamage OnDamage;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnDamaged OnDamaged;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnHeal OnHeal;

	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnHealed OnHealed;

	//Functions

	UFUNCTION(BlueprintCallable, Category = "HealthAlteration")
	void Heal(float healingAmmount);

	UFUNCTION(BlueprintCallable, Category = "HealthAlteration")
	void Damage(float damageAmmount);

	UFUNCTION(BlueprintCallable, Category = "HealthAlteration")
	void MaxHeal();

	UFUNCTION(BlueprintCallable, Category = "HealthAlteration")
	void InstaKill();

	UFUNCTION(BlueprintCallable, Category = "HealthAlteration")
	void CopyHealthBehaviour(UAdvancedHealthBehaviour* otherHealthBehaviour);

	//Properties/Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool useLifeSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int lifes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float health; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
	bool regenerationOverTime;

private:
	float timeScinceLastHealthAlteration = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
	float healthRegenerationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
	float maxHealthRegeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
	float timeToRegenerateHealth;


	// Sets default values for this component's properties
	UAdvancedHealthBehaviour();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
