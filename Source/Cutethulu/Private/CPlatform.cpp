// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlatform.h"

// Sets default values
ACPlatform::ACPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Route = CreateDefaultSubobject<USplineComponent>(TEXT("Route"));
	Route->SetupAttachment(DefaultSceneRoot);

	PlatformOffset = CreateDefaultSubobject<USceneComponent>(TEXT("PlatformOffset"));
	PlatformOffset->SetupAttachment(DefaultSceneRoot);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collision->SetupAttachment(PlatformOffset);

	childActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	childActor->SetupAttachment(PlatformOffset);

	this->lockPitch = true;
	this->lockYaw = false;
	this->lockRoll = true;

}

// Called when the game starts or when spawned
void ACPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FTransform targetTransform;
	double targetRoll = 0, targetPitch = 0, targetYaw = 0;
	FRotator SplineRot, CurrentRot;
	switch (MovementType)
	{
	case EMovementType::ONCE:
	{
		if (splinePos < Route->GetSplineLength()) {
			CalculateSpeed(DeltaTime);
			splinePos += currentSpeed * DeltaTime;
			if (splinePos > Route->GetSplineLength())
				splinePos = Route->GetSplineLength();
			targetTransform = Route->GetTransformAtDistanceAlongSpline(
				splinePos,
				ESplineCoordinateSpace::Local,
				false
			);
			SplineRot = targetTransform.GetRotation().Rotator();
			CurrentRot = PlatformOffset->GetRelativeRotation();
			if (lockRoll)
				SplineRot.Roll = CurrentRot.Roll;
			if (lockPitch)
				SplineRot.Pitch = CurrentRot.Pitch;
			if (lockYaw)
				SplineRot.Yaw = CurrentRot.Yaw;

			PlatformOffset->SetRelativeLocationAndRotation(
				targetTransform.GetLocation(),
				SplineRot
			);
		}
		break;
	}
	case EMovementType::LOOP:
	{
		CalculateSpeed(DeltaTime);
		splinePos += currentSpeed * DeltaTime;
		if (splinePos > Route->GetSplineLength()) {
			splinePos = 0;
			if (ResetSpeedOnLoop)
				currentSpeed = 0;
		}
		else if (reverse && splinePos <= 0) {
			splinePos = Route->GetSplineLength();
			if (ResetSpeedOnLoop)
				currentSpeed = 0;
		}
		targetTransform = Route->GetTransformAtDistanceAlongSpline(
			splinePos,
			ESplineCoordinateSpace::Local,
			false
		);

		SplineRot = targetTransform.GetRotation().Rotator();
		CurrentRot = PlatformOffset->GetRelativeRotation();
		if (lockRoll)
			SplineRot.Roll = CurrentRot.Roll;
		if (lockPitch)
			SplineRot.Pitch = CurrentRot.Pitch;
		if (lockYaw)
			SplineRot.Yaw = CurrentRot.Yaw;

		PlatformOffset->SetRelativeLocationAndRotation(
			targetTransform.GetLocation(),
			SplineRot
		);
		break;
	}
	case EMovementType::PINGPONG:
	{
		CalculateSpeed(DeltaTime);

		splinePos += currentSpeed * DeltaTime;

		const float splineLength = Route->GetSplineLength();

		if (splinePos > splineLength)
		{
			const float excess = splinePos - splineLength;
			splinePos = splineLength - excess;

			reverse = true;

			if (forceOnReverse)
			{
				currentSpeed = 0.0f;
			}
			else if (currentSpeed > 0.0f)
			{
				currentSpeed = -currentSpeed;
			}
		}
		else if (splinePos < 0.0f)
		{
			const float excess = -splinePos;
			splinePos = excess;

			reverse = false;

			if (forceOnReverse)
			{
				currentSpeed = 0.0f;
			}
			else if (currentSpeed < 0.0f)
			{
				currentSpeed = -currentSpeed * DeltaTime;
			}
		}

		targetTransform = Route->GetTransformAtDistanceAlongSpline(
			splinePos,
			ESplineCoordinateSpace::Local,
			false
		);

		SplineRot = targetTransform.GetRotation().Rotator();
		CurrentRot = PlatformOffset->GetRelativeRotation();
		if (lockRoll)
			SplineRot.Roll = CurrentRot.Roll;
		if (lockPitch)
			SplineRot.Pitch = CurrentRot.Pitch;
		if (lockYaw)
			SplineRot.Yaw = CurrentRot.Yaw;

		PlatformOffset->SetRelativeLocationAndRotation(
			targetTransform.GetLocation(),
			SplineRot
		);

		break;
	}
	default:
	{
		break;
	}
	}
}

void ACPlatform::CalculateSpeed(float DeltaTime)
{
	if (!reverse) {
		if (currentSpeed < 0){
			if (forceOnReverse)
				currentSpeed = 0;
			else{
				currentSpeed += deceleration * DeltaTime;
				if (currentSpeed > 0)
					currentSpeed = 0;
			}
		}
		else{
			if (breaking){
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking"));
				currentSpeed -= deceleration * DeltaTime;
				if (currentSpeed < 0)
					currentSpeed = 0;
			}
			else{
				currentSpeed += acceleration * DeltaTime;
				if (currentSpeed > maxSpeed)
					currentSpeed = maxSpeed;
			}
		}
	}
	else {
		if (currentSpeed > 0){ 
			if (forceOnReverse)
				currentSpeed = 0;
			else{
				currentSpeed -= deceleration * DeltaTime;
				if (currentSpeed < 0)
					currentSpeed = 0;
			}
		}
		else{
			if (breaking)	{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking"));
				currentSpeed += deceleration * DeltaTime;
				if (currentSpeed > 0)
					currentSpeed = 0;
			}
			else{
				currentSpeed -= acceleration * DeltaTime;
				if (currentSpeed < -maxSpeed)
					currentSpeed = -maxSpeed;
			}
		}
	}
}

//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Speed: %.2f"), currentSpeed));