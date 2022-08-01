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
	// 여기서 첫 시작시에 필요한 모든 정보를 초기화.
	JRPGSerial.FieldLocation = FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(2518.f, -1835.f, 670.f)); // 첫 시작
	JRPGSerial.RepreCharacterNum = 101;
	JRPGSerial.CurrentParty.Add(101);
	JRPGSerial.CurrentParty.Add(102);
	JRPGSerial.CurrentParty.Add(103);
	JRPGSerial.CurrentFieldNum = 1;

	JRPGSerial.HaveCharList.Add(101);
	JRPGSerial.HaveCharList.Add(102);
	JRPGSerial.HaveCharList.Add(103);

	JRPGSerial.HaveCharStat.Add(101, FJRPGCharStat(1000.f, 100.f, 100.f, 30.f));
	JRPGSerial.HaveCharStat.Add(102, FJRPGCharStat(800.f, 100.f, 110.f, 20.f));
	JRPGSerial.HaveCharStat.Add(103, FJRPGCharStat(700.f, 100.f, 125.f, 25.f));

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
