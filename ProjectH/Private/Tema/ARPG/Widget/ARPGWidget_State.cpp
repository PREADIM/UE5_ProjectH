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
	HP->SetPercent(1.f);
	PrevHP->SetPercent(1.f);
	OwnerUnit->OnDamage.AddUFunction(this, FName("SetHP"));

	AP->SetPercent(1.f);
	PrevAP->SetPercent(0.f);
	OwnerUnit->OnAttackAP.AddUFunction(this, FName("SetAP"));
	AP->PercentDelegate.BindUFunction(this, FName("RetAP"));
}

void UARPGWidget_State::NativeConstruct()
{
	Super::NativeConstruct();

	AP->SynchronizeProperties();
}


void UARPGWidget_State::SetHP()
{
	CurrentHPPercent = OwnerUnit->UnitState.HP / OwnerUnit->UnitState.NormallyHP;
	HP->SetPercent(CurrentHPPercent);

	GetWorld()->GetTimerManager().SetTimer(PrevHPHandle, this, &UARPGWidget_State::SetPrevHP, GetWorld()->GetDeltaSeconds(), true, 1.f);
}

void UARPGWidget_State::SetPrevHP()
{
	LerpHPPercent = FMath::FInterpTo(LerpHPPercent, CurrentHPPercent, GetWorld()->GetDeltaSeconds(), 5.f);
	PrevHP->SetPercent(LerpHPPercent);

	if (LerpHPPercent <= CurrentHPPercent)
		GetWorld()->GetTimerManager().ClearTimer(PrevHPHandle);
}

float UARPGWidget_State::RetAP()
{
	if (!OwnerUnit)
		return 0.0f;

	CurrentAPPercent = OwnerUnit->UnitState.AP / OwnerUnit->UnitState.NormallyAP;

	return CurrentAPPercent;
}


//AP의 경우 실시간으로 줄어드는 경우와, 공격, 히트시에 줄어드는 경우가 다르다.
//중요한 것은이미 AP의 경우 RetAP로 실시간으로 AP를 출력받고 있기때문에
//달리기같은 스태미나가 계속 닳아드는 것에는 PrevAP가 보이면안된다.
//때문에 공격 받을시나 공격할때 PrevAP를 SetPercent 하는 방식으로 한다.
void UARPGWidget_State::SetAP()
{
	if (LerpAPPercent < CurrentAPPercent)
	{
		PrevAP->SetPercent(CurrentAPPercent);
		LerpAPPercent = CurrentAPPercent;
	}

	GetWorld()->GetTimerManager().SetTimer(PrevAPHandle, this, &UARPGWidget_State::SetPrevAP, GetWorld()->GetDeltaSeconds(), true, 0.5f);
}

void UARPGWidget_State::SetPrevAP()
{
	LerpAPPercent = FMath::FInterpTo(LerpAPPercent, CurrentAPPercent, GetWorld()->GetDeltaSeconds(), 13.f);
	PrevAP->SetPercent(LerpAPPercent);

	if (LerpAPPercent <= CurrentAPPercent)
	{
		GetWorld()->GetTimerManager().ClearTimer(PrevAPHandle);
		PrevAP->SetPercent(0.f); // 지속해서 닳는 AP들이 이PrevAP를 못보게 하기위해.
	}

}