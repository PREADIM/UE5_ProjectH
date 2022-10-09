// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGEnermy.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGUnit.h"

// Sets default values
AJRPGEnermy::AJRPGEnermy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.f, 0.f);

	bDead = false;

}

// Called when the game starts or when spawned
void AJRPGEnermy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJRPGEnermy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJRPGEnermy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AJRPGEnermy::EnermyCollisionOverlap(AJRPGPlayerController* PC)
{
	if (PC)
	{
		PC->PlayBattleMode(EnermyUnits);
	}
}


void AJRPGEnermy::PlayerCollisionOverlap(AJRPGUnit* PlayerUnit)
{
	if (PlayerUnit)
	{
		PlayerUnit->OwnerController->PlayBattleMode(EnermyUnits);
	}
}