// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGUnitBase.h"

// Sets default values
AARPGUnitBase::AARPGUnitBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentAttackFactor = 1.0f;
}

// Called when the game starts or when spawned
void AARPGUnitBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARPGUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AARPGUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AARPGUnitBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	
}

float AARPGUnitBase::CalculDamage(float Damage)
{
	return (Damage + UnitState.ATK) * CurrentAttackFactor;
}

float AARPGUnitBase::CalculAPDamage(float APDamage)
{
	float APDMG = APDamage + CurrentAP_DMG;
	_DEBUG("%f", APDMG);
	return APDamage + CurrentAP_DMG;
}

