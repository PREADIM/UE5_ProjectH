// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGEnermy_Mini.h"

AARPGEnermy_Mini::AARPGEnermy_Mini()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARPGEnermy_Mini::BeginPlay()
{
	Super::BeginPlay();
}

void AARPGEnermy_Mini::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARPGEnermy_Mini::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AARPGEnermy_Mini::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AARPGEnermy_Mini::Attack()
{
	if (!AttackMontage)
		return;

	PlayAnimMontage(AttackMontage);
	bAttacking = true;
	bMoving = false;
}

void AARPGEnermy_Mini::Garud()
{

}


// 몽타주 끝나면 하면 될듯.
void AARPGEnermy_Mini::AttackEnd()
{
	bAttacking = false;
	OnAttack.Broadcast();
}


