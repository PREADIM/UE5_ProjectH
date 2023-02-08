// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Widget/ARPGWidget_BossHPView.h"
#include "Components/ProgressBar.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Components/TextBlock.h"

void UARPGWidget_BossHPView::Init(AARPGUnitBase* Unit)
{
	Super::Init(Unit);
	MaxDamageRenderDelay = 5.f;
	LerpHPPercent = 1.f;
}

void UARPGWidget_BossHPView::NativeConstruct()
{
	Super::NativeConstruct();

	if (!OwnerUnit && !BossUnit)
	{
		_DEBUG("Boss HP Not OwnerUnit");
		return;
	}

	// 이게 안되면 과연 BP에서 위젯 블프로 만든것을 넣었는지 의심해보자.
	/*BossHP->PercentDelegate.BindUFunction(this, FName("RetHP"));
	BossHP->SynchronizeProperties();*/

	// 어처피 데미지를 입는 수치는 같으므로 그냥 둘다 델리게이트에 실행
	BossHP->SetPercent(1.f);
	BossHP_Prev->SetPercent(1.f);
	BossUnit->OnDamage.AddUFunction(this, FName("SetHP"));
	BossUnit->OnDamage.AddUFunction(this, FName("SetTextDamage"));
	DamageText->SetRenderOpacity(0.f);
}


void UARPGWidget_BossHPView::SetHP()
{
	CurrentHPPercent = BossUnit->UnitState.HP / BossUnit->UnitState.NormallyHP;
	BossHP->SetPercent(CurrentHPPercent);

	/*if (!GetWorld()->GetTimerManager().IsTimerActive(PrevHPHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(PrevHPHandle, this, &UARPGWidget_BossHPView::SetPrevHP, GetWorld()->GetDeltaSeconds(), true, 1.f);
	}*/

	// ClearTime을 하지않아도 어처피 SetTimer를 하면 새롭게 씌여진다.
	GetWorld()->GetTimerManager().SetTimer(PrevHPHandle, this, &UARPGWidget_BossHPView::SetPrevHP, GetWorld()->GetDeltaSeconds(), true, 1.f);
}

void UARPGWidget_BossHPView::SetPrevHP()
{
	LerpHPPercent = FMath::FInterpTo(LerpHPPercent, CurrentHPPercent, GetWorld()->GetDeltaSeconds(), 3.f);
	BossHP_Prev->SetPercent(LerpHPPercent);

	if(LerpHPPercent <= CurrentHPPercent)
		GetWorld()->GetTimerManager().ClearTimer(PrevHPHandle);
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


