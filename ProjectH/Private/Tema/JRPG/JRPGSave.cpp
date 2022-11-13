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
	HaveCharList.Add(Num);
	HaveCharLevels.Add(Num, 1);
	CharStats.Add(Num, FJRPGCharStat());

	CurrentExp.Add(Num, 0.0f);
	NextExp.Add(Num, 0.0f);
}


FJRPGFieldEnermy::FJRPGFieldEnermy()
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
	JRPGSerial.CurrentParty.Add(102);
	JRPGSerial.CurrentParty.Add(103);

	JRPGSerial.SetCharNum(101); // �������ִ� ĳ���Ϳ�, ���� ����
	JRPGSerial.SetCharNum(102);
	JRPGSerial.SetCharNum(103);

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

	OwnerController->NextExp = JRPGSerial.NextExp;
	OwnerController->CurrentExp = JRPGSerial.CurrentExp;

	// �ڡڡ� NextExp�� CharStats�� ���̺긦 �����ʰ�, ���� ���۽� ������ ���̺��� ������ ���Ȱ����� �����Ѵ�.
	// ���� ���� ���� �Ǵ� ���� ����ؾ��ϱ� ����. �̹� ����Ǿ������� �����Ͱ� �̻�������.
	// �ڡ� ������ ��ó�� Add�� ���� Ű���� �����ؾ��ϴ� ���� �ؾ��ϹǷ� ���̺� ��ü�� �صд�. (Ű�������� ����.)
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
	JRPGSerial.NextExp = OwnerController->NextExp;

	SaveSlot();
}


void UJRPGSave::SetFieldEnermy(class AJRPGEnermy* FieldEnermy, int32 KillCnt)
{
	if (JRPGFieldEnermy.FieldEnermyIsLive.Find(FieldEnermy->FieldEnermyNumber))
	{
		JRPGFieldEnermy.FieldEnermyIsLive[FieldEnermy->FieldEnermyNumber] = FieldEnermy->bDead;
	}
	else
	{
		JRPGFieldEnermy.FieldEnermyIsLive.Add(FieldEnermy->FieldEnermyNumber, FieldEnermy->bDead);
	}

	JRPGFieldEnermy.KillCnt = KillCnt;
	SaveSlot();
}


bool UJRPGSave::GetFieldEnermy(int32 FieldEnermyNum)
{
	if (JRPGFieldEnermy.FieldEnermyIsLive.Find(FieldEnermyNum))
	{
		return JRPGFieldEnermy.FieldEnermyIsLive[FieldEnermyNum];
	}
	else
	{
		JRPGFieldEnermy.FieldEnermyIsLive.Add(FieldEnermyNum, false);
		return false;
	}
}

float UJRPGSave::GetKillCnt()
{
	return JRPGFieldEnermy.KillCnt;
}


void UJRPGSave::SaveSlot()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}
