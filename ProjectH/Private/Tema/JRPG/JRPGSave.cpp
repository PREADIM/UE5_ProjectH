// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGSave.h"
#include "Tema/JRPG/JRPGPlayerController.h"

const FString UJRPGSave::SlotName = FString("JRPGSave");


FJRPGSerial::FJRPGSerial()
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
	JRPGSerial.CurrentParty.Add(101);
	JRPGSerial.CurrentParty.Add(102);
	JRPGSerial.CurrentParty.Add(103);
	JRPGSerial.CurrentFieldNum = 1;
	JRPGSerial.HaveCharList.Add(101);
	JRPGSerial.HaveCharStat.Add(101, FJRPGCharStat(1000.f, 100.f, 100.f, 30.f));

	SaveSlot();
}

void UJRPGSave::SetLoadCharacter(AJRPGPlayerController* OwnerController)
{
	OwnerController->FieldLocation = JRPGSerial.FieldLocation;
	OwnerController->RepreCharacterNum = JRPGSerial.RepreCharacterNum;
	OwnerController->CurrentParty = JRPGSerial.CurrentParty;
	OwnerController->CurrentFieldNum = JRPGSerial.CurrentFieldNum;
	OwnerController->HaveCharList = JRPGSerial.HaveCharList;
	OwnerController->HaveCharStat = JRPGSerial.HaveCharStat;
}


void UJRPGSave::SetSave(class AJRPGPlayerController* OwnerController)
{
	JRPGSerial.FieldLocation = OwnerController->FieldLocation;
	JRPGSerial.RepreCharacterNum = OwnerController->RepreCharacterNum;
	JRPGSerial.CurrentParty = OwnerController->CurrentParty;
	JRPGSerial.CurrentFieldNum = OwnerController->CurrentFieldNum;
	JRPGSerial.HaveCharList = OwnerController->HaveCharList;
	JRPGSerial.HaveCharStat = OwnerController->HaveCharStat;

	_DEBUG("%d !!", JRPGSerial.RepreCharacterNum);
	SaveSlot();
}


void UJRPGSave::SaveSlot()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}
