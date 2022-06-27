// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGSave.h"
#include "Tema/JRPG/JRPGPlayerController.h"

const FString UJRPGSave::SlotName = FString("JRPGSave");


UJRPGSave::UJRPGSave()
{
	// ���⼭ ù ���۽ÿ� �ʿ��� ��� ������ �ʱ�ȭ.
	FieldLocation = FTransform(FRotator(0.0f, 0.0f, 90.0f), FVector(2518.f, -1835.f, 670.f)); // ù ����
	RepreCharacter = 101;
	CurrentParty.Add(101);
	CurrnetFieldNum = 1;
	HaveCharList.Add(101);
	HaveCharStat.Add(101, FJRPGCharStat(1000, 100, 100, 30));
}

void UJRPGSave::SetLoadCharacter(AJRPGPlayerController* OwnerController)
{
	OwnerController->FieldLocation = FieldLocation;
	OwnerController->RepreCharacterNum = RepreCharacter;
	OwnerController->CurrentParty = CurrentParty;
	OwnerController->CurrentFieldNum = CurrnetFieldNum;
	OwnerController->HaveCharList = HaveCharList;
	OwnerController->HaveCharStat = HaveCharStat;

}


void UJRPGSave::SetSave(class AJRPGPlayerController* OwnerController)
{
	FieldLocation = OwnerController->FieldLocation;
	RepreCharacter = OwnerController->RepreCharacterNum;
	CurrentParty = OwnerController->CurrentParty;
	CurrnetFieldNum = OwnerController->CurrentFieldNum;
	HaveCharList = OwnerController->HaveCharList;
	HaveCharStat = OwnerController->HaveCharStat;
}
