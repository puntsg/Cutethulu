// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlatform.h"
#include "GameFramework/Character.h"

// Sets default values
ACPlatform::ACPlatform()
{
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

	lockPitch = true;
	lockYaw = false;
	lockRoll = true;
	bWaitingForPlayer = false;
}

void ACPlatform::BeginPlay()
{
	Super::BeginPlay();

	remainingTimeToActivate = 0.0f;
	bWaitingForPlayer = false;

	if (activateWhenPlayerLands)
	{
		isWaiting = true;
		bWaitingForPlayer = true;
	}

	if (MovementType == EMovementType::SEQUENCE && previousPlatform != nullptr)
	{
		isWaiting = true;
		bWaitingForPlayer = false;
		remainingTimeToActivate = -1.0f;
	}

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACPlatform::OnCollisionBeginOverlap);
}

void ACPlatform::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherActor->IsA<ACharacter>())
		return;

	if (bWaitingForPlayer)
	{
		ActivatePlatform();
	}
}

void ACPlatform::ActivatePlatform()
{
	isWaiting = false;
	bWaitingForPlayer = false;
	remainingTimeToActivate = 0.0f;
}

void ACPlatform::ResetPlatformPosition()
{
	splinePos = 0;
	UpdateTransform();
}

// Called every frame
void ACPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isWaiting)
	{
		if (bWaitingForPlayer)
			return;

		if (remainingTimeToActivate < 0.0f)
			return;

		remainingTimeToActivate -= DeltaTime;
		if (remainingTimeToActivate <= 0.0f)
		{
			isWaiting = false;
			remainingTimeToActivate = 0.0f;
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
	if (!reverse)
	{
		if (currentSpeed < 0)
		{
			if (forceOnReverse)
				currentSpeed = 0;
			else
			{
				currentSpeed += deceleration * DeltaTime;
				if (currentSpeed > 0)
					currentSpeed = 0;
			}
		}
		else
		{
			if (breaking)
			{
				currentSpeed -= deceleration * DeltaTime;
				if (currentSpeed < 0)
					currentSpeed = 0;
			}
			else
			{
				currentSpeed += acceleration * DeltaTime;
				if (currentSpeed > maxSpeed)
					currentSpeed = maxSpeed;
			}
		}
	}
	else
	{
		if (currentSpeed > 0)
		{
			if (forceOnReverse)
				currentSpeed = 0;
			else
			{
				currentSpeed -= deceleration * DeltaTime;
				if (currentSpeed < 0)
					currentSpeed = 0;
			}
		}
		else
		{
			if (breaking)
			{
				currentSpeed += deceleration * DeltaTime;
				if (currentSpeed > 0)
					currentSpeed = 0;
			}
			else
			{
				currentSpeed -= acceleration * DeltaTime;
				if (currentSpeed < -maxSpeed)
					currentSpeed = -maxSpeed;
			}
		}
	}
}

void ACPlatform::UpdateTransform()
{
	FTransform targetTransform = Route->GetTransformAtDistanceAlongSpline(
		splinePos,
		ESplineCoordinateSpace::Local,
		false
	);

	FRotator SplineRot = targetTransform.GetRotation().Rotator();
	FRotator CurrentRot = PlatformOffset->GetRelativeRotation();

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

void ACPlatform::OnceMovement(float DeltaTime)
{
	const float splineLength = Route->GetSplineLength();

	if (!reverse)
	{
		if (splinePos < splineLength)
		{
			CalculateSpeed(DeltaTime);
			splinePos += currentSpeed * DeltaTime;
			if (splinePos > splineLength)
				splinePos = splineLength;
			UpdateTransform();
		}
	}
	else
	{
		if (splinePos > 0)
		{
			CalculateSpeed(DeltaTime);
			splinePos += currentSpeed * DeltaTime;
			if (splinePos < 0)
				splinePos = 0;
			UpdateTransform();
		}
	}
}

void ACPlatform::LoopMovement(float DeltaTime)
{
	const float splineLength = Route->GetSplineLength();

	CalculateSpeed(DeltaTime);
	splinePos += currentSpeed * DeltaTime;

	if (!reverse)
	{
		if (splinePos > splineLength)
		{
			splinePos = 0;
			if (ResetSpeedOnLoop)
				currentSpeed = 0;
			RestoreWaiting();
		}
	}
	else
	{
		if (splinePos < 0)
		{
			splinePos = splineLength;
			if (ResetSpeedOnLoop)
				currentSpeed = 0;
			RestoreWaiting();
		}
	}

	UpdateTransform();
}

void ACPlatform::PingPongMovement(float DeltaTime)
{
	const float splineLength = Route->GetSplineLength();

	CalculateSpeed(DeltaTime);
	splinePos += currentSpeed * DeltaTime;

	if (splinePos > splineLength)
	{
		const float excess = splinePos - splineLength;
		splinePos = splineLength - excess;
		reverse = true;

		if (forceOnReverse)
			currentSpeed = 0.0f;
		else
			currentSpeed = -FMath::Abs(currentSpeed);

		RestoreWaiting();
	}
	else if (splinePos < 0.0f)
	{
		const float excess = -splinePos;
		splinePos = excess;
		reverse = false;

		if (forceOnReverse)
			currentSpeed = 0.0f;
		else
			currentSpeed = FMath::Abs(currentSpeed);

		RestoreWaiting();
	}

	UpdateTransform();
}

void ACPlatform::SequenceMovement(float DeltaTime)
{
	const float splineLength = Route->GetSplineLength();

	CalculateSpeed(DeltaTime);
	splinePos += currentSpeed * DeltaTime;

	if (splinePos >= splineLength)
	{
		splinePos = splineLength;
		currentSpeed = 0.0f;

		if (nextPlatform != nullptr)
		{
			if (nextPlatform->waitingTime > 0)
			{
				nextPlatform->isWaiting = true;
				nextPlatform->SetRemainingTimeToActivate(nextPlatform->waitingTime);
				nextPlatform->SetWaitingForPlayer(false);
			}
			else
			{
				nextPlatform->isWaiting = false;
			}
			nextPlatform->reverse = false;

			isWaiting = true;
			remainingTimeToActivate = -1.0f;
		}
		else
		{
			if (waitForPlayerOnSequenceEnd)
			{
				reverse = true;
				isWaiting = true;
				bWaitingForPlayer = true;
			}
			else
			{
				const float excess = splinePos - splineLength;
				splinePos = splineLength - excess;
				reverse = true;
				if (forceOnReverse)
					currentSpeed = 0.0f;
				else
					currentSpeed = -FMath::Abs(currentSpeed);
				RestoreWaiting();
			}
		}
	}
	else if (splinePos <= 0.0f)
	{
		splinePos = 0.0f;
		currentSpeed = 0.0f;

		if (previousPlatform != nullptr)
		{
			if (previousPlatform->waitingTime > 0)
			{
				previousPlatform->isWaiting = true;
				previousPlatform->SetRemainingTimeToActivate(previousPlatform->waitingTime);
				previousPlatform->SetWaitingForPlayer(false);
			}
			else
			{
				previousPlatform->isWaiting = false;
			}
			previousPlatform->reverse = true;

			isWaiting = true;
			remainingTimeToActivate = -1.0f;
		}
		else
		{
			if (waitForPlayerOnSequenceEnd)
			{
				reverse = false;
				isWaiting = true;
				bWaitingForPlayer = true;
			}
			else
			{
				const float excess = -splinePos;
				splinePos = excess;
				reverse = false;
				if (forceOnReverse)
					currentSpeed = 0.0f;
				else
					currentSpeed = FMath::Abs(currentSpeed);
				RestoreWaiting();
			}
		}
	}

	UpdateTransform();
}

void ACPlatform::RestoreWaiting()
{
	if (waitingTime > 0)
	{
		isWaiting = true;
		remainingTimeToActivate = waitingTime;
		bWaitingForPlayer = false;
	}

	if (activateWhenPlayerLands)
	{
		isWaiting = true;
		bWaitingForPlayer = true;
	}
}

//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Speed: %.2f"), currentSpeed));