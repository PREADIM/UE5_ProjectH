// Fill out your copyright notice in the Description page of Project Settings.
#include "Tema/ARPG/Widget/ARPGWidget_State.h"
#include "Components/ProgressBar.h"
#include "Tema/ARPG/ARPGUnitBase.h"

void UARPGWidget_State::Init(AARPGUnitBase* Unit)
{
	Super::Init(Unit);

	if (!OwnerUnit)
		return;

	LerpHPPercent = 1.f;
}

void UARPGWidget_State::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerUnit->OnDamage.AddUFunction(this, FName("SetHP"));
	HP->SetPercent(1.f);
	AP->PercentDelegate.BindUFunction(this, FName("RetAP"));
	AP->SynchronizeProperties();
}


void UARPGWidget_State::SetHP()
{
	CurrentHPPercent = OwnerUnit->UnitState.HP / OwnerUnit->UnitState.NormallyHP;
	HP->SetPercent(CurrentHPPercent);

	if (GetWorld()->GetTimerManager().IsTimerActive(PrevHPHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(PrevHPHandle);
	}

	GetWorld()->GetTimerManager().SetTimer(PrevHPHandle, this, &UARPGWidget_State::SetPrevHP, GetWorld()->GetDeltaSeconds(), true, 4.f);
}

void UARPGWidget_State::SetPrevHP()
{
	LerpHPPercent = FMath::FInterpTo(LerpHPPercent, CurrentHPPercent, GetWorld()->GetDeltaSeconds(), 3.f);
	PrevHP->SetPercent(LerpHPPercent);

	if (LerpHPPercent <= CurrentHPPercent)
		GetWorld()->GetTimerManager().ClearTimer(PrevHPHandle);
}

float UARPGWidget_State::RetAP()
{
	if (!OwnerUnit)
		return 0.0f;

	return OwnerUnit->UnitState.AP / OwnerUnit->UnitState.NormallyAP;
}
