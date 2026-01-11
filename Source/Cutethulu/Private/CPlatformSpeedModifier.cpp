// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlatformSpeedModifier.h"
#include <CPlatform.h>

// Sets default values
ACPlatformSpeedModifier::ACPlatformSpeedModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(DefaultSceneRoot);
	
}

// Called when the game starts or when spawned
void ACPlatformSpeedModifier::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACPlatformSpeedModifier::OnOverlap);
}

void ACPlatformSpeedModifier::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlapped"));
	if(ACPlatform* overlappedPlatform = Cast<ACPlatform>(OtherActor)){
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlapped actor is a platform"));
		if (this->inverseReverseValue)
			overlappedPlatform->reverse = !overlappedPlatform->reverse;
		else if (this->ReverseValue.applyValue)
			overlappedPlatform->reverse = this->ReverseValue.Value;

		if (!overlappedPlatform->reverse) {
			if (this->setBreakingValue.applyValue)
				overlappedPlatform->breaking = this->setBreakingValue.Value;
			if (this->newCurrentSpeed.applyValue)
				overlappedPlatform->currentSpeed = this->newCurrentSpeed.Value;
			if (this->newMaxSpeed.applyValue)
				overlappedPlatform->maxSpeed = this->newMaxSpeed.Value;
			if (this->newAcceleration.applyValue)
				overlappedPlatform->acceleration = this->newAcceleration.Value;
			if (this->newDeceleration.applyValue)
				overlappedPlatform -> deceleration = this->newDeceleration.Value;
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlapped actor is a reversed platform"));
			if (this->setReverseBreakingValue.applyValue)
				overlappedPlatform->breaking = this->setReverseBreakingValue.Value;
			if (this->newCurrentSpeed.applyValue)
				overlappedPlatform->currentSpeed = this->newReverseCurrentSpeed.Value;
			if (this->newReverseMaxSpeed.applyValue)
				overlappedPlatform->maxSpeed = this->newReverseMaxSpeed.Value;
			if (this->newReverseAcceleration.applyValue)
				overlappedPlatform->acceleration = this->newReverseAcceleration.Value;
			if (this->newReverseDeceleration.applyValue)
				overlappedPlatform->deceleration = this->newReverseDeceleration.Value;
		}
	}
}

// Called every frame
void ACPlatformSpeedModifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

