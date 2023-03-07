// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/SaveGame.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "JRPGSave.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FJRPGSerial
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		FTransform FieldLocation;
	UPROPERTY(VisibleAnywhere)
		int32 RepreCharacterNum;

	UPROPERTY(VisibleAnywhere)
		TArray<int32> CurrentParty; // ���� ���õǾ��ִ� ��Ƽ����Ʈ
	UPROPERTY(VisibleAnywhere)
		int32 CurrentFieldNum; // �ʵ� ����
	UPROPERTY(VisibleAnywhere)
		TMap<int32, float> CurrentExp; // ���� ����ġ

	UPROPERTY(VisibleAnywhere)
		TArray<int32> HaveCharList; // ������ �ִ� ��ü ĳ���� �ѹ� (���� ���̺� �ε� �ؾ���).
	// ���߿� ����Ʈ �ѹ�ó�� ��ȣ�� BP �н��� ������ �ش� �ѹ��� �����ϰ� �ҷ����� �������� �����ɵ�.
	UPROPERTY(VisibleAnywhere)
		TMap<int32, int32> HaveCharLevels;
	UPROPERTY(VisibleAnywhere)
		TMap<int32, FJRPGCharStat> CharStats; // ������ �ִ� ĳ������ ���ݵ�.


public:
	FJRPGSerial();
	void SetCharNum(int32 Num);

};


USTRUCT(BlueprintType)
struct FJRPGFieldEnermy
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		TMap<int32, bool> FieldEnermyIsLive;
	UPROPERTY(VisibleAnywhere)
		int32 KillCnt;
	UPROPERTY(VisibleAnywhere)
		bool bTutorial = false;
	UPROPERTY(VisibleAnywhere)
		bool bPartyTutorial = false;

	FJRPGFieldEnermy();

};




UCLASS()
class PROJECTH_API UJRPGSave : public USaveGame
{
	GENERATED_BODY()


public:
	UJRPGSave();
	void FirstSave();

public:
	static const FString SlotName;

	UPROPERTY(VisibleAnywhere)
		FJRPGSerial JRPGSerial;
	UPROPERTY(VisibleAnywhere)
		FJRPGFieldEnermy JRPGFieldEnermy;

public:
	void SetLoadCharacter(class AJRPGPlayerController* OwnerController);
	void SetSave(class AJRPGPlayerController* OwnerController);

	void SetFieldEnermy(class AJRPGEnermy* FieldEnermy, int32 KillCnt);
	bool GetFieldEnermy(int32 FieldEnermyNum);
	float GetKillCnt();
	void SetBattleTutorial(); // Ʃ�丮�� ��.
	void SetPartyTutorial();

	void SaveSlot();
};
