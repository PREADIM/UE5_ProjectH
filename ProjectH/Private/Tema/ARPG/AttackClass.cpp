// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AttackClass.h"
#include "Tema/ARPG/ARPGUnitBase.h"


void UAttackClass::Init()
{
	AttackFunc.Clear();
	CppAttackFunc.Clear();
	OwnerUnit = nullptr;
	AttackStruct.CurrentCnt = 0;
	AttackStruct.bCanThisAttack = true;
}

void UAttackClass::PlayAttack()
{
	if (OwnerUnit == nullptr)
	{
		_DEBUG("OwnerUnit nullptr");
		return;
	}

	OwnerUnit->CurrentAttackFactor = AttackStruct.DamageFactor;
	OwnerUnit->CurrentAP_DMG = AttackStruct.AP_DMG;
	AttackStruct.CurrentCnt++;

	BindFunction();
	// 공격 클래스 마다 따로 설정해둔 함수 실행
	
	if (CppAttackFunc.IsBound())
	{
		CppAttackFunc.Broadcast();
		_DEBUG("Cpp Broadcast");
	}


	if (AttackFunc.IsBound())
	{
		AttackFunc.Broadcast();
		_DEBUG("BP Broadcast");
	}
}

void UAttackClass::SetCanThisAttack(bool bFlag)
{
	AttackStruct.bCanThisAttack = bFlag;
}