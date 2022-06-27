// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/SaveGame.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "JRPGSave.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UJRPGSave : public USaveGame
{
	GENERATED_BODY()


public:
	UJRPGSave();

public:
	static const FString SlotName;

	UPROPERTY()
		FTransform FieldLocation;
	UPROPERTY()
		int32 RepreCharacter;

	UPROPERTY()
		TArray<int32> CurrentParty; // ���� ���õǾ��ִ� ��Ƽ����Ʈ
	UPROPERTY()
		int32 CurrnetFieldNum; // �ʵ� ����
	

	UPROPERTY()
		TArray<int32> HaveCharList; // ������ �ִ� ��ü ĳ���� �ѹ� (���� ���̺� �ε� �ؾ���).
	// ���߿� ����Ʈ �ѹ�ó�� ��ȣ�� BP �н��� ������ �ش� �ѹ��� �����ϰ� �ҷ����� �������� �����ɵ�.
	UPROPERTY()
		TMap<int32, FJRPGCharStat> HaveCharStat; // ������ �ִ� ĳ������ ���ݵ�.


public:
	void SetLoadCharacter(class AJRPGPlayerController* OwnerController);
	void SetSave(class AJRPGPlayerController* OwnerController);
};
