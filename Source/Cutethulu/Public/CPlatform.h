// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include <Engine/Engine.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <Components/SplineComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>

#include "CPlatform.generated.h"

UENUM(BlueprintType)
enum class EMovementType : uint8 {
	NONE	UMETA(DisplayName = "None"),
	ONCE UMETA(DisplayName = "Once"),
	LOOP UMETA(DisplayName = "Loop"),
	PINGPONG UMETA(DisplayName = "PingPong")
};

UCLASS(Blueprintable, BlueprintType)
class CUTETHULU_API ACPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPlatform();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<USplineComponent> Route;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> PlatformOffset;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> Collision;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UChildActorComponent> childActor;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams")
	EMovementType MovementType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/SpeedParams ", meta = (Units = "Km/h", ToolTip = "Velocidad actual de la plataforma"))
	float currentSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/SpeedParams ", meta = (Units = "Km/h", ToolTip = "Aceleración de la plataforma"))
	float acceleration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/SpeedParams ", meta = (Units = "Km/h", ToolTip = "Velocidad de frenado de la plataforma"))
	float deceleration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/SpeedParams ", meta = (Units = "Km/h", ToolTip="Velocidad máxima de la plataforma, en caso de reversa, se invertirá el valor PE: 5 ->-5"))
	float maxSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/SpeedParams ", meta = (Units = "Km", ToolTip="Que tanto ha avanzado en el spline, ni caso"))
	float splinePos;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/SpeedParams ", meta = ( ToolTip = "Recorre en sentido inverso"))
	bool reverse;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/SpeedParams ", meta = ( ToolTip = "Aplica freno y desaceleración a la plataforma"))
	bool breaking;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/SpeedParams ", meta = (ToolTip = "Al dar una vuelta, la velocidad se mantiene o pasa a 0"))
	bool ResetSpeedOnLoop;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/SpeedParams ", meta = (ToolTip = "Si al activar reverse la plataforma va desacelerando o fuerza su velocidad a 0"))
	bool forceOnReverse;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/RotationParams ", meta = (ToolTip = "Bloquea la rotación en Pitch"))
	bool lockPitch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/RotationParams ", meta = (ToolTip = "Bloquea la rotación en Yaw"))
	bool lockYaw;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams/RotationParams ", meta = (ToolTip = "Bloquea la rotación en Roll"))
	bool lockRoll;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void CalculateSpeed(float DeltaTime);
};
