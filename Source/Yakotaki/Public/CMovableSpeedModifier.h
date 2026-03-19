// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/BoxComponent.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMovableSpeedModifier.generated.h"

USTRUCT(BlueprintType)
struct FValueSetter {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Sobreescribir valor?"))
	bool applyValue;
};

USTRUCT(BlueprintType)
struct FFloatSetter : public FValueSetter {
	GENERATED_BODY()
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Valor a sobreescribir"))
	float Value;
};

USTRUCT(BlueprintType)
struct FBoolSetter : public FValueSetter {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Valor a sobreescribir"))
	bool Value;
};

UCLASS()
class YAKOTAKI_API ACMovableSpeedModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACMovableSpeedModifier();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> Collider;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Invertir dirección de la plataforma"))
	bool inverseReverseValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Definir dirección de la plataforma"))
	FBoolSetter ReverseValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Fijar si frena o no"))
	FBoolSetter setBreakingValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Fijar si frena o no mientras está en reversa"))
	FBoolSetter setReverseBreakingValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Sobreescribir velocidad de la plataforma"))
	FFloatSetter newCurrentSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Sobreescribir velocidad de la plataforma en reversa"))
	FFloatSetter newReverseCurrentSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Sobreescribir velocidad max de la plataforma"))
	FFloatSetter newMaxSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Sobreescribir velocidad max de la plataforma en reversa"))
	FFloatSetter newReverseMaxSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Sobreescribir aceleración de la plataforma"))
	FFloatSetter newAcceleration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Sobreescribir aceleración de la plataforma en reversa"))
	FFloatSetter newReverseAcceleration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Sobreescribir desaceleración de la plataforma"))
	FFloatSetter newDeceleration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " ", meta = (ToolTip = "Sobreescribir desaceleración de la plataforma en reversa"))
	FFloatSetter newReverseDeceleration;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
