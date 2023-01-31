// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Weapon/ARPGWeapon.h"

// Sets default values
AARPGWeapon::AARPGWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ARPGUnitChannel = ECollisionChannel::ECC_GameTraceChannel12;
}

// Called when the game starts or when spawned
void AARPGWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARPGWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AController* AARPGWeapon::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) 
		return nullptr;

	return OwnerPawn->GetController();
}


