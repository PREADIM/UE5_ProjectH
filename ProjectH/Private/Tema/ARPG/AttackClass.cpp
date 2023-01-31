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

}

void UAttackClass::PlayAttack(UWorld* UnitWorld)
{
	if (OwnerUnit == nullptr || UnitWorld == nullptr)
	{
		_DEBUG("OwnerUnit nullptr");
		return;
	}

	if(World == nullptr)
		World = UnitWorld;

	OwnerUnit->CurrentAttackFactor = AttackStruct.DamageFactor;
	OwnerUnit->CurrentAP_DMG = AttackStruct.AP_DMG;
	AttackStruct.CurrentCnt++;

	// 공격 클래스 마다 따로 설정해둔 함수 실행
	BindFunction();
	CoolTimeStart();
	
	// 델리게이트의 경우. 현재 사용하지 않음.
	/*if (CppAttackFunc.IsBound())
	{
		CppAttackFunc.Broadcast();
		_DEBUG("Cpp Broadcast");
	}*/
}

void UAttackClass::CoolTimeStart()
{
	if (bCoolTime)
	{
		SetCanThisAttack(false);
		World->GetTimerManager().SetTimer(CoolTimeHandle, this, &UAttackClass::CoolTimeFunc, 1.f, true);
	}
}

// OwnerUnit->GetWorld 한 이유는 UAttackClass는 월드에 스폰되지않은 형태가 없는 클래스이기때문에
// World 자체가 없기때문. 때문에 월드에 존재하는 OwnerUnit의 world를 참조해야한다.

void UAttackClass::CoolTimeFunc()
{
	if (CoolTime <= CurrentCoolTime)
	{	
		CurrentCoolTime = 0.f;
		SetCanThisAttack(true);
		World->GetTimerManager().ClearTimer(CoolTimeHandle);
		_DEBUG("CoolTime Clear");
	}
	else
	{
		CurrentCoolTime += 1.f;
	}
}

void UAttackClass::SetCanThisAttack(bool bFlag)
{
	AttackStruct.bCanThisAttack = bFlag;
}