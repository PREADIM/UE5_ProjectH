// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Widget/ARPGWidget_BattleHP.h"
#include "Components/ProgressBar.h"
#include "Tema/ARPG/ARPGUnitBase.h"
#include "Components/TextBlock.h"

void UARPGWidget_BattleHP::Init(AARPGUnitBase* Unit)
{
	Super::Init(Unit);

	MaxDamageRenderDelay = 5.f;
	MaxRenderDelay = 15.f; // 체력바 딜레이
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

	BattleHP->PercentDelegate.BindUFunction(this, FName("RetHP"));
	BattleHP->SynchronizeProperties();

	OwnerUnit->OnDamage.AddUFunction(this, FName("SetTextDamage"));
	Damage->SetRenderOpacity(0.f);

}

float UARPGWidget_BattleHP::RetHP()
{
	if (!OwnerUnit)
		return 0.f;

	return OwnerUnit->UnitState.HP / OwnerUnit->UnitState.NormallyHP;
}

void UARPGWidget_BattleHP::SetTextDamage(float TakeDamage)
{
	int32 IntDamage = FMath::FloorToInt(PrevDamage + TakeDamage);
	
	Delay = 0.f;
	Damage->SetText(FText::FromString(FString::FromInt(IntDamage)));

	if (!GetWorld()->GetTimerManager().IsTimerActive(DmgRenderTimer))
	{
		Damage->SetRenderOpacity(1.0f);
		GetWorld()->GetTimerManager().SetTimer(DmgRenderTimer, this, &UARPGWidget_BattleHP::RenderDamageText, 1.0f, true);	
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
	else if (Delay >= MaxDamageRenderDelay)
	{
		Damage->SetRenderOpacity(0.0f);
		PrevDamage = 0.f;
	}
	else
	{
		Delay += 1.f;
	}
}