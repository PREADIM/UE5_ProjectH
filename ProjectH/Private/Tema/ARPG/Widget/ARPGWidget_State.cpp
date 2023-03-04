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


//AP�� ��� �ǽð����� �پ��� ����, ����, ��Ʈ�ÿ� �پ��� ��찡 �ٸ���.
//�߿��� �����̹� AP�� ��� RetAP�� �ǽð����� AP�� ��¹ް� �ֱ⶧����
//�޸��ⰰ�� ���¹̳��� ��� ��Ƶ�� �Ϳ��� PrevAP�� ���̸�ȵȴ�.
//������ ���� �����ó� �����Ҷ� PrevAP�� SetPercent �ϴ� ������� �Ѵ�.
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
		PrevAP->SetPercent(0.f); // �����ؼ� ��� AP���� ��PrevAP�� ������ �ϱ�����.
	}

}