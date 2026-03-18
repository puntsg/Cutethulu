// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include <Engine/Engine.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

#include "CPlatform.generated.h"

UENUM(BlueprintType)
enum class EMovementType : uint8 {
	NONE		UMETA(DisplayName = "None"),
	ONCE		UMETA(DisplayName = "Once"),
	LOOP		UMETA(DisplayName = "Loop"),
	PINGPONG	UMETA(DisplayName = "PingPong"),
	SEQUENCE	UMETA(DisplayName = "Sequence")
};

UCLASS(Blueprintable, BlueprintType)
class CUTETHULU_API ACPlatform : public AActor
{
	GENERATED_BODY()

public:
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams", meta = (ToolTip = "La plataforma espera a que el jugador la pise para activarse"))
	bool activateWhenPlayerLands;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SpeedParams", meta = (ToolTip = "Velocidad actual de la plataforma"))
	float currentSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SpeedParams", meta = (ToolTip = "Aceleracion de la plataforma"))
	float acceleration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SpeedParams", meta = (ToolTip = "Velocidad de frenado de la plataforma"))
	float deceleration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SpeedParams", meta = (ToolTip = "Velocidad maxima de la plataforma"))
	float maxSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SpeedParams", meta = (ToolTip = "Posicion actual en el spline"))
	float splinePos;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SpeedParams", meta = (ToolTip = "Recorre en sentido inverso"))
	bool reverse;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SpeedParams", meta = (ToolTip = "Aplica freno a la plataforma"))
	bool breaking;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SpeedParams", meta = (ToolTip = "Al completar un ciclo, la velocidad se reinicia a 0"))
	bool ResetSpeedOnLoop;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SpeedParams", meta = (ToolTip = "Al cambiar de direccion, fuerza velocidad a 0 en vez de desacelerar"))
	bool forceOnReverse;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|RotationParams", meta = (ToolTip = "Bloquea la rotacion en Pitch"))
	bool lockPitch;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|RotationParams", meta = (ToolTip = "Bloquea la rotacion en Yaw"))
	bool lockYaw;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|RotationParams", meta = (ToolTip = "Bloquea la rotacion en Roll"))
	bool lockRoll;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|TimingParams", meta = (ToolTip = "La plataforma esta en espera"))
	bool isWaiting;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|TimingParams", meta = (ToolTip = "Tiempo de espera entre recorridos (en segundos)"))
	float waitingTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SequenceParams", meta = (ToolTip = "Siguiente plataforma en la secuencia"))
	ACPlatform* nextPlatform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SequenceParams", meta = (ToolTip = "Plataforma anterior en la secuencia"))
	ACPlatform* previousPlatform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlatformParams|SequenceParams", meta = (ToolTip = "Al acabar la secuencia sin siguiente/anterior, espera al jugador para invertir"))
	bool waitForPlayerOnSequenceEnd;

protected:
	virtual void BeginPlay() override;
	float remainingTimeToActivate;
	bool bWaitingForPlayer;

public:
	virtual void Tick(float DeltaTime) override;

	void SetRemainingTimeToActivate(float time) { remainingTimeToActivate = time; }
	float GetRemainingTimeToActivate() const { return remainingTimeToActivate; }

	void SetWaitingForPlayer(bool bWaiting) { bWaitingForPlayer = bWaiting; }
	bool IsWaitingForPlayer() const { return bWaitingForPlayer; }

	UFUNCTION(BlueprintCallable)
	void ActivatePlatform();

	UFUNCTION(BlueprintCallable)
	void ResetPlatformPosition();

private:
	void CalculateSpeed(float DeltaTime);
	void OnceMovement(float DeltaTime);
	void LoopMovement(float DeltaTime);
	void PingPongMovement(float DeltaTime);
	void SequenceMovement(float DeltaTime);
	void RestoreWaiting();
	void UpdateTransform(float DeltaTime);

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};