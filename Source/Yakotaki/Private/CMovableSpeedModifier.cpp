// Fill out your copyright notice in the Description page of Project Settings.


#include "CMovableSpeedModifier.h"
#include <CMovable.h>

// Sets default values
ACMovableSpeedModifier::ACMovableSpeedModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(DefaultSceneRoot);
	
}

// Called when the game starts or when spawned
void ACMovableSpeedModifier::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACMovableSpeedModifier::OnOverlap);
}

void ACMovableSpeedModifier::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlapped"));
	if(ACMovable* overlappedMovable = Cast<ACMovable>(OtherActor)){
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlapped actor is a Movable"));
		if (this->inverseReverseValue) {
			overlappedMovable->reverse = !overlappedMovable->reverse;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,	FString::Printf(TEXT("Reversing reverse value to %s"),overlappedMovable->reverse ? TEXT("true") : TEXT("false")));
		}
		else if (this->ReverseValue.applyValue) {
			overlappedMovable->reverse = this->ReverseValue.Value;
		}
		if (!overlappedMovable->reverse) {
			if (this->setBreakingValue.applyValue){
				overlappedMovable->breaking = this->setBreakingValue.Value;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Setting breaking to %s" ),this->setBreakingValue.Value);
			}
			if (this->newCurrentSpeed.applyValue) {
				overlappedMovable->currentSpeed = this->newCurrentSpeed.Value;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking"));
			}
			if (this->newMaxSpeed.applyValue) {
				overlappedMovable->maxSpeed = this->newMaxSpeed.Value;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking"));
			}
			if (this->newAcceleration.applyValue) {
				overlappedMovable->acceleration = this->newAcceleration.Value;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking"));
			}
			if (this->newDeceleration.applyValue) {
				overlappedMovable->deceleration = this->newDeceleration.Value;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking"));
			}
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlapped actor is a reversed Movable"));
			if (this->setReverseBreakingValue.applyValue) {
				overlappedMovable->breaking = this->setReverseBreakingValue.Value;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking"));
			}
			if (this->newReverseCurrentSpeed.applyValue) {
				overlappedMovable->currentSpeed = this->newReverseCurrentSpeed.Value;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking"));
			}
			if (this->newReverseMaxSpeed.applyValue) {
				overlappedMovable->maxSpeed = this->newReverseMaxSpeed.Value;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking"));
			}
			if (this->newReverseAcceleration.applyValue) {
				overlappedMovable->acceleration = this->newReverseAcceleration.Value;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking"));
			}
			if (this->newReverseDeceleration.applyValue) {
				overlappedMovable->deceleration = this->newReverseDeceleration.Value;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking"));
			}
		}
	}
}

// Called every frame
void ACMovableSpeedModifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

