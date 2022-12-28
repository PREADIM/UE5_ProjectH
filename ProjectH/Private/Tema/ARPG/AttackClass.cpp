// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AttackClass.h"
#include "GameFramework/Character.h"


void UAttackClass::Init()
{
	AttackFunc.Clear();
	CppAttackFunc.Clear();
	AttackStruct.CurrentCnt = 0;
	AttackStruct.bCanThisAttack = true;
}

void UAttackClass::PlayAttack()
{
	AttackStruct.CurrentCnt++;
	
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