// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGSave.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGEnermy.h"

const FString UJRPGSave::SlotName = FString("JRPGSave");


FJRPGSerial::FJRPGSerial()
{

}

void FJRPGSerial::SetCharNum(int32 Num)
{
	HaveCharList.Emplace(Num);
	HaveCharLevels.Emplace(Num, 1);
	CharStats.Emplace(Num, FJRPGCharStat());
	CurrentExp.Emplace(Num, 0.0f);
}


FJRPGFieldEnermySave::FJRPGFieldEnermySave()
{

}


UJRPGSave::UJRPGSave()
{

}

void UJRPGSave::FirstSave()
{
	// 여기서 첫 시작시에 필요한 모든 정보를 초기화.
	JRPGSerial.FieldLocation = FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(2518.f, -1835.f, 670.f)); // 첫 시작
	JRPGSerial.RepreCharacterNum = 101;

	JRPGSerial.CurrentParty.Add(101); // 현재 등록한 파티의 정보.
	//JRPGSerial.CurrentParty.Add(102);
	//JRPGSerial.CurrentParty.Add(103);

	JRPGSerial.SetCharNum(101); // 가지고있는 캐릭터와, 스텟 저장
	//JRPGSerial.SetCharNum(102);
	//JRPGSerial.SetCharNum(103);

	JRPGSerial.CurrentFieldNum = 1;


	SaveSlot();
}

void UJRPGSave::SetLoadCharacter(AJRPGPlayerController* OwnerController)
{
	OwnerController->FieldLocation = JRPGSerial.FieldLocation;
	OwnerController->RepreCharacterNum = JRPGSerial.RepreCharacterNum;
	OwnerController->CurrentParty = JRPGSerial.CurrentParty;
	OwnerController->CurrentFieldNum = JRPGSerial.CurrentFieldNum;
	OwnerController->HaveCharList = JRPGSerial.HaveCharList;
	OwnerController->HaveCharLevels = JRPGSerial.HaveCharLevels;
	OwnerController->CharStats = JRPGSerial.CharStats;

	OwnerController->CurrentExp = JRPGSerial.CurrentExp;

	// ★ CharStats은 세이브를 하지않고, 게임 시작시 데이터 테이블에서 가져온 스탯값으로 저장한다.
	// 추후 값이 변경 되는 것을 우려해야하기 때문. 이미 저장되어있으면 데이터가 이상해진다.
	// ★ 하지만 어처피 Add를 통해 키값을 저장해야하는 것을 해야하므로 세이브 자체는 해둔다. (키값때문에 저장.)
}


void UJRPGSave::SetSave(class AJRPGPlayerController* OwnerController)
{
	JRPGSerial.FieldLocation = OwnerController->FieldLocation;
	JRPGSerial.RepreCharacterNum = OwnerController->RepreCharacterNum;
	JRPGSerial.CurrentParty = OwnerController->CurrentParty;
	JRPGSerial.CurrentFieldNum = OwnerController->CurrentFieldNum;
	JRPGSerial.HaveCharList = OwnerController->HaveCharList;
	JRPGSerial.HaveCharLevels = OwnerController->HaveCharLevels;
	JRPGSerial.CharStats = OwnerController->CharStats;
	JRPGSerial.CurrentExp = OwnerController->CurrentExp;

	SaveSlot();
}


void UJRPGSave::SetFieldEnermy(class AJRPGFieldEnermy* FieldEnermy, int32 KillCnt)
{
	if (JRPGFieldEnermySave.FieldEnermyIsLive.Find(FieldEnermy->FieldEnermyNumber))
		JRPGFieldEnermySave.FieldEnermyIsLive[FieldEnermy->FieldEnermyNumber] = FieldEnermy->bDead;
	else
		JRPGFieldEnermySave.FieldEnermyIsLive.Add(FieldEnermy->FieldEnermyNumber, FieldEnermy->bDead);

	JRPGFieldEnermySave.KillCnt = KillCnt;
	SaveSlot();
}


bool UJRPGSave::GetFieldEnermy(int32 FieldEnermyNum)
{
	if (JRPGFieldEnermySave.FieldEnermyIsLive.Find(FieldEnermyNum))
		return JRPGFieldEnermySave.FieldEnermyIsLive[FieldEnermyNum];
	else
	{
		JRPGFieldEnermySave.FieldEnermyIsLive.Add(FieldEnermyNum, false);
		return false;
	}
}

float UJRPGSave::GetKillCnt()
{
	return JRPGFieldEnermySave.KillCnt;
}

void UJRPGSave::SetBattleTutorial()
{
	JRPGFieldEnermySave.bTutorial = true;
}

void UJRPGSave::SetPartyTutorial()
{
	JRPGFieldEnermySave.bPartyTutorial = true;
}


void UJRPGSave::SaveSlot()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}
