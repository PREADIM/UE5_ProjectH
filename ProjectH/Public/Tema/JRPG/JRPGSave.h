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
		TArray<int32> HaveCharList; // ������ �ִ� ��ü ĳ���� �ѹ� (���� ���̺� �ε� �ؾ���).
	// ���߿� ����Ʈ �ѹ�ó�� ��ȣ�� BP �н��� ������ �ش� �ѹ��� �����ϰ� �ҷ����� �������� �����ɵ�.
	UPROPERTY(VisibleAnywhere)
		TMap<int32, FJRPGCharStat> HaveCharStat; // ������ �ִ� ĳ������ ���ݵ�.


public:
	FJRPGSerial();

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

public:
	void SetLoadCharacter(class AJRPGPlayerController* OwnerController);
	void SetSave(class AJRPGPlayerController* OwnerController);
	void SaveSlot();

	
	
};
