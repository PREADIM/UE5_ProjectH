// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGUnitBase.h"

AJRPGUnitBase::AJRPGUnitBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AJRPGUnitBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJRPGUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetPhysicalSound();

}

void AJRPGUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AJRPGUnitBase::SetPhysicalSound()
{
	TEnumAsByte<EPhysicalSurface> PS = TracePysicalSurface(this, SurfaceDistance);

	if (!PhysicalAllSounds.Find(PS))
		return;

	PhysicalSounds = PhysicalAllSounds[PS]; // 해당하는 표면의 사운드 가져오기
}
