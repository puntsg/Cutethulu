// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedHealthBehaviour.h"

// Sets default values for this component's properties
UAdvancedHealthBehaviour::UAdvancedHealthBehaviour()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called when the game starts
void UAdvancedHealthBehaviour::BeginPlay()
{
	Super::BeginPlay();
	this->health = this->maxHealth;
	this->OnHealthAltered.Broadcast(this->health,this->maxHealth);
	
}


// Called every frame
void UAdvancedHealthBehaviour::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UAdvancedHealthBehaviour::MaxHeal()
{
	this->Heal(this->maxHealth-this->health);
}

void UAdvancedHealthBehaviour::Heal(float healingAmmount)
{
	this->OnHeal.Broadcast(this->health,this->maxHealth,healingAmmount);
	this->OnHealthAlter.Broadcast(this->health, this->maxHealth);
	this->health += healingAmmount;
	if (this->health > this->maxHealth)
		this->health = this->maxHealth;
	this->OnHealed.Broadcast(this->health, this->maxHealth, healingAmmount);
	this->OnHealthAltered.Broadcast(this->health, this->maxHealth);
}

void UAdvancedHealthBehaviour::InstaKill() {
	this->Damage(this->health);
}
void UAdvancedHealthBehaviour::Damage(float damageAmmount)
{
	this->OnHealthAlter.Broadcast(this->health, this->maxHealth);
	this->OnDamage.Broadcast(this->health, this->maxHealth, damageAmmount);
	this->health -= damageAmmount;
	if (this->health <= 0)
	{
		this->OnDie.Broadcast();
		if (this->useLifeSystem) {
			this->lifes--;
			this->OnLifeConsumed.Broadcast(this->lifes);
			if (this->lifes < 0) {
				this->OnAllLifesConsumed.Broadcast();
			}
		}
	}
	this->OnDamaged.Broadcast(this->health, this->maxHealth, damageAmmount);
	this->OnHealthAltered.Broadcast(this->health, this->maxHealth);
}

void UAdvancedHealthBehaviour::CopyHealthBehaviour(UAdvancedHealthBehaviour* otherHealthBehaviour, bool mantainRatio)
{
	this->health = otherHealthBehaviour->health;
	this->maxHealth = otherHealthBehaviour->maxHealth;
	this->lifes = otherHealthBehaviour->lifes;
	this->regenerationOverTime = otherHealthBehaviour->regenerationOverTime;
	this->healthRegenerationSpeed = otherHealthBehaviour->healthRegenerationSpeed;
	this->maxHealthRegeneration = otherHealthBehaviour->maxHealthRegeneration;
}

void UAdvancedHealthBehaviour::AddLifes(int lifesToAdd)
{
	this->lifes += lifesToAdd;
	if (this->lifes < 0)
		this->lifes = 0;
}

void UAdvancedHealthBehaviour::AddLife()
{
	this->lifes++;
}

void UAdvancedHealthBehaviour::QuitLife()
{
	this->lifes--;
}

void UAdvancedHealthBehaviour::SetLifes(int ammount)
{
	this->lifes = ammount;
	if (this->lifes < 0)
		this->lifes = 0;
}



void UAdvancedHealthBehaviour::SetMaxHealth(float ammount, bool addHealth, bool fillHealth) {
	this->maxHealth += ammount;
	if (addHealth)
		this->Heal(ammount);
	else if (fillHealth)
		this->MaxHeal();
}
void UAdvancedHealthBehaviour::AddMaxHealth(float ammount, bool addHealth, bool fillHealth) {
	this->SetMaxHealth(this->maxHealth + ammount, addHealth, fillHealth);
}
void UAdvancedHealthBehaviour::Regenerate() {
	this->Heal(this->regenerationValue);
	if (this->health >= maxHealthRegeneration) {
		GetWorld()->GetTimerManager().ClearTimer(RegenerationTimer);
		this->health = maxHealthRegeneration;
	}
}
void UAdvancedHealthBehaviour::SetRegenerationEvent() {
	GetWorld()->GetTimerManager().ClearTimer(RegenerationTimer);
	GetWorld()->GetTimerManager().SetTimer(
		RegenerationTimer,
		this,
		&UAdvancedHealthBehaviour::Regenerate,
		this->timeBetweenRegeneration,
		true,
		this->timeToStartRegeneratingHealth
	);
}

