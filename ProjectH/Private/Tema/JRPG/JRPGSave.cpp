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
	// ���⼭ ù ���۽ÿ� �ʿ��� ��� ������ �ʱ�ȭ.
	JRPGSerial.FieldLocation = FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(2518.f, -1835.f, 670.f)); // ù ����
	JRPGSerial.RepreCharacterNum = 101;

	JRPGSerial.CurrentParty.Add(101); // ���� ����� ��Ƽ�� ����.
	//JRPGSerial.CurrentParty.Add(102);
	//JRPGSerial.CurrentParty.Add(103);

	JRPGSerial.SetCharNum(101); // �������ִ� ĳ���Ϳ�, ���� ����
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

	// �� CharStats�� ���̺긦 �����ʰ�, ���� ���۽� ������ ���̺��� ������ ���Ȱ����� �����Ѵ�.
	// ���� ���� ���� �Ǵ� ���� ����ؾ��ϱ� ����. �̹� ����Ǿ������� �����Ͱ� �̻�������.
	// �� ������ ��ó�� Add�� ���� Ű���� �����ؾ��ϴ� ���� �ؾ��ϹǷ� ���̺� ��ü�� �صд�. (Ű�������� ����.)
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
