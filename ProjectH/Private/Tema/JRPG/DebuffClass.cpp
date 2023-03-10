// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/DebuffClass.h"

uint32 GetTypeHash(const FDebuffStruct& DebuffStruct)
{
	return FCrc::MemCrc32(&DebuffStruct, sizeof(DebuffStruct));
}


void FCCState::SetCCType(ECCType CCType, bool bFlag)
{
	switch (CCType)
	{
	case ECCType::NONE:
		break;
	case ECCType::STUN:
		bStun = bFlag;
		LastCCType = CCType;
		break;
	}
}


FDebuffStruct::FDebuffStruct(FString AttackUnitName, class UDebuffClass* AttackDebuff)
	: InstigatorUnitName(AttackUnitName), DebuffClass(AttackDebuff)
{
	if (DebuffClass)
		DebuffClass->CurrentDebuffTurn = DebuffClass->DebuffTurn;
}

bool UDebuffClass::SetupCnt()
{
	CurrentDebuffTurn -= 1;
	if (CurrentDebuffTurn == 0)
		return true;
	else
		return false;
}
