// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AttackClass.h"
#include "Tema/ARPG/ARPGUnitBase.h"


void UAttackClass::Init()
{
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
	if (bAttackCoolTime)
	{
		SetCanThisAttack(false);
		World->GetTimerManager().SetTimer(CoolTimeHandle, this, &UAttackClass::CoolTimeFunc, CoolTime, false);
	}
}

// OwnerUnit->GetWorld �� ������ UAttackClass�� ���忡 ������������ ���°� ���� Ŭ�����̱⶧����
// World ��ü�� ���⶧��. ������ ���忡 �����ϴ� OwnerUnit�� world�� �����ؾ��Ѵ�.

void UAttackClass::CoolTimeFunc()
{
	SetCanThisAttack(true);
}

void UAttackClass::SetCanThisAttack(bool bFlag)
{
	AttackStruct.bCanThisAttack = bFlag;
}