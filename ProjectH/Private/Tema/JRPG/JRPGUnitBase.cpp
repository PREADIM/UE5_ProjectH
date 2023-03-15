// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGUnitBase.h"

// Sets default values
AJRPGUnitBase::AJRPGUnitBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJRPGUnitBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJRPGUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJRPGUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

