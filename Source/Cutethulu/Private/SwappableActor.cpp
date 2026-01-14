// Fill out your copyright notice in the Description page of Project Settings.


#include "SwappableActor.h"

// Sets default values
ASwappableActor::ASwappableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASwappableActor::BeginPlay()
{
	Super::BeginPlay();
	
}
void ASwappableActor::Swap() {

}
// Called every frame
void ASwappableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

