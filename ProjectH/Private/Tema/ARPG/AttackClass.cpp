// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AttackClass.h"
#include "Tema/ARPG/ARPGUnitBase.h"


void UAttackClass::Init()
{
	//CppAttackFunc.Clear();
	OwnerUnit = nullptr;
	AttackStruct.CurrentCnt = 0;
	AttackStruct.bCanThisAttack = true;
	EffectCnt = Effects.Num();
	SoundCnt = Sounds.Num();
	AttackSoundCnt = AttackSounds.Num();
}

void UAttackClass::PlayAttack(UWorld* UnitWorld)
{
	if (OwnerUnit == nullptr || UnitWorld == nullptr)
		return;

	if(World == nullptr)
		World = UnitWorld;

	OwnerUnit->CurrentAttackFactor = AttackStruct.DamageFactor;
	OwnerUnit->CurrentAP_DMG = AttackStruct.AP_DMG;
	AttackStruct.CurrentCnt++;

	// ���� Ŭ���� ���� ���� �����ص� �Լ� ����
	BindFunction();
	CoolTimeStart();

}

void UAttackClass::CoolTimeStart()
{
	if (bCoolTime)
	{
		SetCanThisAttack(false);
		//World->GetTimerManager().SetTimer(CoolTimeHandle, this, &UAttackClass::CoolTimeFunc, 1.f, true);
		World->GetTimerManager().SetTimer(CoolTimeHandle, this, &UAttackClass::CoolTimeFunc, CoolTime, false);
	}
}

// OwnerUnit->GetWorld �� ������ UAttackClass�� ���忡 ������������ ���°� ���� Ŭ�����̱⶧����
// World ��ü�� ���⶧��. ������ ���忡 �����ϴ� OwnerUnit�� world�� �����ؾ��Ѵ�.

void UAttackClass::CoolTimeFunc()
{
	/*if (CoolTime <= CurrentCoolTime)
	{	
		CurrentCoolTime = 0.f;
		SetCanThisAttack(true);
		if (World)
		{
			World->GetTimerManager().ClearTimer(CoolTimeHandle);
		}
	}
	else
	{
		CurrentCoolTime += 1.f;
	}*/

	SetCanThisAttack(true);
}

void UAttackClass::SetCanThisAttack(bool bFlag)
{
	AttackStruct.bCanThisAttack = bFlag;
}