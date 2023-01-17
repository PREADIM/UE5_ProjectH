// Fill out your copyright notice in the Description page of Project Settings.
#include "Tema/ARPG/Widget/ARPGWidget_State.h"
#include "Components/ProgressBar.h"
#include "Tema/ARPG/ARPGUnitBase.h"

void UARPGWidget_State::NativeConstruct()
{
	Super::NativeConstruct();

	HP->PercentDelegate.BindUFunction(this, FName("RetHP"));
	AP->PercentDelegate.BindUFunction(this, FName("RetAP"));
	HP->SynchronizeProperties();
	AP->SynchronizeProperties();
}

void UARPGWidget_State::Init(AARPGUnitBase* Unit)
{
	Super::Init(Unit);

	if (!OwnerUnit)
		return;

}

float UARPGWidget_State::RetHP()
{
	if (!OwnerUnit)
		return 0.0f;

	return OwnerUnit->UnitState.HP / OwnerUnit->UnitState.NormallyHP;
}

float UARPGWidget_State::RetAP()
{
	if (!OwnerUnit)
		return 0.0f;

	return OwnerUnit->UnitState.AP / OwnerUnit->UnitState.NormallyAP;
}
