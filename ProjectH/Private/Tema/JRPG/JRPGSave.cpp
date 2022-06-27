// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGSave.h"
#include "Tema/JRPG/JRPGPlayerController.h"

const FString UJRPGSave::SlotName = FString("JRPGSave");


FJRPGSerial::FJRPGSerial()
{

}


UJRPGSave::UJRPGSave()
{
	// 여기서 첫 시작시에 필요한 모든 정보를 초기화.
	FJRPGSerial Temp;

	Temp.FieldLocation = FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(2518.f, -1835.f, 670.f)); // 첫 시작
	Temp.RepreCharacter = 101;
	Temp.CurrentParty.Add(101);
	Temp.CurrentFieldNum = 1;
	Temp.HaveCharList.Add(101);
	Temp.HaveCharStat.Add(101, FJRPGCharStat(1000, 100, 100, 30));

	JRPGSerial = Temp;
}

void UJRPGSave::SetLoadCharacter(AJRPGPlayerController* OwnerController)
{
	OwnerController->FieldLocation = JRPGSerial.FieldLocation;
	OwnerController->RepreCharacterNum = JRPGSerial.RepreCharacter;
	OwnerController->CurrentParty = JRPGSerial.CurrentParty;
	OwnerController->CurrentFieldNum = JRPGSerial.CurrentFieldNum;
	OwnerController->HaveCharList = JRPGSerial.HaveCharList;
	OwnerController->HaveCharStat = JRPGSerial.HaveCharStat;

}


void UJRPGSave::SetSave(class AJRPGPlayerController* OwnerController)
{
	JRPGSerial.FieldLocation = OwnerController->FieldLocation;
	JRPGSerial.RepreCharacter = OwnerController->RepreCharacterNum;
	JRPGSerial.CurrentParty = OwnerController->CurrentParty;
	JRPGSerial.CurrentFieldNum = OwnerController->CurrentFieldNum;
	JRPGSerial.HaveCharList = OwnerController->HaveCharList;
	JRPGSerial.HaveCharStat = OwnerController->HaveCharStat;

	SaveSlot();
}


void UJRPGSave::SaveSlot()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}