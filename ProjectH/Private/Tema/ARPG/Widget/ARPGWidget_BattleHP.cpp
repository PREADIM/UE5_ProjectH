// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Widget/ARPGWidget_BattleHP.h"
#include "Components/ProgressBar.h"
#include "Tema/ARPG/ARPGUnitBase.h"
#include "Components/TextBlock.h"

void UARPGWidget_BattleHP::Init(AARPGUnitBase* Unit)
{
	Super::Init(Unit);

	MaxDamageRenderDelay = 5.f;
	MaxRenderDelay = 10.f; // 체력바 딜레이
	LerpHPPercent = 1.f;
	Delay = 0.f;
}

void UARPGWidget_BattleHP::NativeConstruct()
{
	Super::NativeConstruct();

	if (!OwnerUnit)
	{
		_DEBUG("Battle HP Not OwnerUnit");
		return;
	}

	BattleHP->SetPercent(1.f);
	BattleHP_Prev->SetPercent(1.f);
	OwnerUnit->OnDamage.AddUFunction(this, FName("SetHP"));
	OwnerUnit->OnDamage.AddUFunction(this, FName("SetTextDamage"));
	Damage->SetRenderOpacity(0.f);
}

void UARPGWidget_BattleHP::SetHP()
{
	CurrentHPPercent = OwnerUnit->UnitState.HP / OwnerUnit->UnitState.NormallyHP;
	BattleHP->SetPercent(CurrentHPPercent);

	/*if (!GetWorld()->GetTimerManager().IsTimerActive(PrevHPHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(PrevHPHandle, this, &UARPGWidget_BattleHP::SetPrevHP, GetWorld()->GetDeltaSeconds(), true, 2.f);
	}*/

	// ClearTime을 하지않아도 어처피 SetTimer를 하면 새롭게 씌여진다.
	GetWorld()->GetTimerManager().SetTimer(PrevHPHandle, this, &UARPGWidget_BattleHP::SetPrevHP, GetWorld()->GetDeltaSeconds(), true, 2.f);
}

void UARPGWidget_BattleHP::SetPrevHP()
{
	LerpHPPercent = FMath::FInterpTo(LerpHPPercent, CurrentHPPercent, GetWorld()->GetDeltaSeconds(), 3.f);
	BattleHP_Prev->SetPercent(LerpHPPercent);

	if (LerpHPPercent <= CurrentHPPercent)
		GetWorld()->GetTimerManager().ClearTimer(PrevHPHandle);
}

void UARPGWidget_BattleHP::SetTextDamage(float TakeDamage)
{
	int32 IntDamage = FMath::FloorToInt(PrevDamage + TakeDamage);
	
	Delay = 0.f;
	Damage->SetText(FText::FromString(FString::FromInt(IntDamage)));

	if (!GetWorld()->GetTimerManager().IsTimerActive(DmgRenderTimer))
	{
		SetRenderOpacity(1.0f);
		Damage->SetRenderOpacity(1.0f);
		GetWorld()->GetTimerManager().SetTimer(DmgRenderTimer, this, &UARPGWidget_BattleHP::RenderDamageText, 1.0f, true);	
	}
	else
	{
		if(Damage->GetRenderOpacity() == 0.f)
			Damage->SetRenderOpacity(1.0f);
	}

	PrevDamage = IntDamage;
}


void UARPGWidget_BattleHP::RenderDamageText()
{
	if (Delay >= MaxRenderDelay)
	{
		SetRenderOpacity(0.f);
		GetWorld()->GetTimerManager().ClearTimer(DmgRenderTimer);		
	}
	else if (Damage->GetRenderOpacity() == 1.f && Delay >= MaxDamageRenderDelay)
	{
		Damage->SetRenderOpacity(0.0f);
		PrevDamage = 0.f;
	}
	
	Delay += 1.f;
}