// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Widget/ARPGWidget_BossHPView.h"
#include "Components/ProgressBar.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Components/TextBlock.h"

void UARPGWidget_BossHPView::Init(AARPGUnitBase* Unit)
{
	Super::Init(Unit);
	MaxDamageRenderDelay = 5.f;
}

void UARPGWidget_BossHPView::NativeConstruct()
{
	Super::NativeConstruct();

	if (!OwnerUnit && !BossUnit)
	{
		_DEBUG("Boss HP Not OwnerUnit");
		return;
	}

	// 이게 안되면 과연 블프로 만든것을 넣었는지 의심해보자.
	BossHP->PercentDelegate.BindUFunction(this, FName("RetHP"));
	BossHP->SynchronizeProperties();

	BossUnit->OnDamage.AddUFunction(this, FName("SetTextDamage"));
	_DEBUG("AddUFUNCTION");
	DamageText->SetRenderOpacity(0.f);
}

float UARPGWidget_BossHPView::RetHP()
{
	if (!BossUnit)
		return 0.f;

	return BossUnit->UnitState.HP / BossUnit->UnitState.NormallyHP;
}

void UARPGWidget_BossHPView::SetTextDamage(float TakeDamage)
{
	int32 IntDamage = FMath::FloorToInt(PrevDamage + TakeDamage);

	_DEBUG("SetTextDamage");
	Delay = 0.f;
	DamageText->SetText(FText::FromString(FString::FromInt(IntDamage)));

	if (!GetWorld()->GetTimerManager().IsTimerActive(DmgRenderTimer))
	{
		DamageText->SetRenderOpacity(1.0f);
		GetWorld()->GetTimerManager().SetTimer(DmgRenderTimer, this, &UARPGWidget_BossHPView::RenderDamageText, 1.0f, true);
	}
	else
	{
		if (DamageText->GetRenderOpacity() == 0.f)
			DamageText->SetRenderOpacity(1.0f);
	}

	PrevDamage = IntDamage;
}

void UARPGWidget_BossHPView::RenderDamageText()
{
	if (DamageText->GetRenderOpacity() == 1.f && Delay >= MaxDamageRenderDelay)
	{
		DamageText->SetRenderOpacity(0.0f);
		PrevDamage = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(DmgRenderTimer);
	}
	else
	{
		Delay += 1.f;
	}
}


