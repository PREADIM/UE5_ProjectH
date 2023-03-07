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
		TArray<int32> CurrentParty; // 현재 선택되어있는 파티리스트
	UPROPERTY(VisibleAnywhere)
		int32 CurrentFieldNum; // 필드 정보
	UPROPERTY(VisibleAnywhere)
		TMap<int32, float> CurrentExp; // 현재 경험치

	UPROPERTY(VisibleAnywhere)
		TArray<int32> HaveCharList; // 가지고 있는 전체 캐릭터 넘버 (추후 세이브 로드 해야함).
	// 나중에 퀘스트 넘버처럼 번호와 BP 패스를 가지고 해당 넘버만 저장하고 불러오는 형식으로 만들어도될듯.
	UPROPERTY(VisibleAnywhere)
		TMap<int32, int32> HaveCharLevels;
	UPROPERTY(VisibleAnywhere)
		TMap<int32, FJRPGCharStat> CharStats; // 가지고 있는 캐릭터의 스텟들.


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
	void SetBattleTutorial(); // 튜토리얼 끝.
	void SetPartyTutorial();

	void SaveSlot();
};
