// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlatform.h"
#include "GameFramework/Character.h"

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
	Collision->SetGenerateOverlapEvents(true);

	childActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	childActor->SetupAttachment(PlatformOffset);

	this->lockPitch = true;
	this->lockYaw = false;
	this->lockRoll = true;
}

void ACPlatform::BeginPlay()
{
	Super::BeginPlay();
	remainingTimeToActivate = waitingTime;

	if (activateWhenPlayerLands)
		isWaiting = true;

	if (MovementType == EMovementType::SEQUENCE && previousPlatform != nullptr)
		isWaiting = true;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACPlatform::OnCollisionBeginOverlap);
}

void ACPlatform::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (activateWhenPlayerLands && OtherActor && OtherActor->IsA<ACharacter>())
	{
		isWaiting = false;
		activateWhenPlayerLands = false;
	}
}

// Called every frame
void ACPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isWaiting) {
		remainingTimeToActivate -= DeltaTime;
		if (remainingTimeToActivate <= 0) {
			isWaiting = false;
			remainingTimeToActivate = 0;
		}
		return;
	}
	switch (MovementType)
	{
	case EMovementType::ONCE:
		OnceMovement(DeltaTime);
		break;
	case EMovementType::LOOP:
		LoopMovement(DeltaTime);
		break;
	case EMovementType::PINGPONG:
		PingPongMovement(DeltaTime);
		break;
	case EMovementType::SEQUENCE:
		SequenceMovement(DeltaTime);
		break;
	default:
		break;
	}
}

void ACPlatform::CalculateSpeed(float DeltaTime)
{
	if (!reverse) {
		if (currentSpeed < 0) {
			if (forceOnReverse)
				currentSpeed = 0;
			else {
				currentSpeed += deceleration * DeltaTime;
				if (currentSpeed > 0)
					currentSpeed = 0;
			}
		}
		else {
			if (breaking) {
				currentSpeed -= deceleration * DeltaTime;
				if (currentSpeed < 0)
					currentSpeed = 0;
			}
			else {
				currentSpeed += acceleration * DeltaTime;
				if (currentSpeed > maxSpeed)
					currentSpeed = maxSpeed;
			}
		}
	}
	else {
		if (currentSpeed > 0) {
			if (forceOnReverse)
				currentSpeed = 0;
			else {
				currentSpeed -= deceleration * DeltaTime;
				if (currentSpeed < 0)
					currentSpeed = 0;
			}
		}
		else {
			if (breaking) {
				currentSpeed += deceleration * DeltaTime;
				if (currentSpeed > 0)
					currentSpeed = 0;
			}
			else {
				currentSpeed -= acceleration * DeltaTime;
				if (currentSpeed < -maxSpeed)
					currentSpeed = -maxSpeed;
			}
		}
	}
}

void ACPlatform::OnceMovement(float DeltaTime)
{
	if (splinePos < Route->GetSplineLength()) {
		FTransform targetTransform;
		FRotator SplineRot, CurrentRot;

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
}

void ACPlatform::LoopMovement(float DeltaTime)
{
	FTransform targetTransform;
	FRotator SplineRot, CurrentRot;

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
}

void ACPlatform::PingPongMovement(float DeltaTime)
{
	FTransform targetTransform;
	FRotator SplineRot, CurrentRot;
	CalculateSpeed(DeltaTime);

	splinePos += currentSpeed * DeltaTime;

	const float splineLength = Route->GetSplineLength();

	if (splinePos > splineLength || splinePos < 0.0f)
	{
		if (splinePos > splineLength) {
			const float excess = splinePos - splineLength;
			splinePos = splineLength - excess;
		}
		else if (splinePos < 0.0f) {
			const float excess = -splinePos;
			splinePos = excess;
		}

		reverse = !reverse;
		RestoreWaiting();

		if (forceOnReverse)
		{
			currentSpeed = 0.0f;
		}
		else if (currentSpeed > 0.0f)
		{
			currentSpeed = -currentSpeed;
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
}

void ACPlatform::SequenceMovement(float DeltaTime)
{
	FTransform targetTransform;
	FRotator SplineRot, CurrentRot;
	CalculateSpeed(DeltaTime);

	splinePos += currentSpeed * DeltaTime;

	const float splineLength = Route->GetSplineLength();

	if (splinePos >= splineLength)
	{
		if (nextPlatform != nullptr)
		{
			splinePos = splineLength;
			currentSpeed = 0.0f;
			nextPlatform->isWaiting = false;
			nextPlatform->reverse = false;
			isWaiting = true;
		}
		else
		{
			const float excess = splinePos - splineLength;
			splinePos = splineLength - excess;
			reverse = true;
			if (forceOnReverse)
				currentSpeed = 0.0f;
			else
				currentSpeed = -currentSpeed;
			RestoreWaiting();
		}
	}
	else if (splinePos <= 0.0f)
	{
		if (previousPlatform != nullptr)
		{
			splinePos = 0.0f;
			currentSpeed = 0.0f;
			previousPlatform->isWaiting = false;
			previousPlatform->reverse = true;
			isWaiting = true;
		}
		else
		{
			const float excess = -splinePos;
			splinePos = excess;
			reverse = false;
			if (forceOnReverse)
				currentSpeed = 0.0f;
			else
				currentSpeed = -currentSpeed;
			RestoreWaiting();
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
}

void ACPlatform::RestoreWaiting()
{
	if (waitingTime > 0) {
		isWaiting = true;
		remainingTimeToActivate = waitingTime;
	}
}

//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Speed: %.2f"), currentSpeed));