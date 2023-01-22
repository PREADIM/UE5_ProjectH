// Fill out your copyright notice in the Description page of Project Settings.

#include "Tema/ARPG/Widget/ARPGWidgetMain.h"
#include "Tema/ARPG/Widget/ARPGWidget_State.h"
#include "Tema/ARPG/Widget/ARPGWidget_Charge.h"

void UARPGWidgetMain::Init(AARPGUnitBase* Unit)
{
	Super::Init(Unit);
	State->Init(Unit);
	ChargeAttack->Init(Unit);
}

void UARPGWidgetMain::SetChargeAttacking(float Ratio)
{
	ChargeAttack->BindCharge(Ratio);
}

void UARPGWidgetMain::ChargeAttackInViewport(bool bFlag)
{
	if (bFlag)
	{
		if (ChargeAttack->GetRenderOpacity() == 0.f)
			ChargeAttack->SetRenderOpacity(1.f);
	}
	else
	{
		ChargeAttack->SetRenderOpacity(0.f);
	}
}
