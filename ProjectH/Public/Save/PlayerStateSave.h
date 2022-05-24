// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/SaveGame.h"
#include "PlayerStateSave.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UPlayerStateSave : public USaveGame
{
	GENERATED_BODY()

public:
	static const FString SlotName;

	/*-------------------
		Quest Numbers
	--------------------*/
	UPROPERTY(VisibleAnywhere)
		TArray<int32> QuestNumbers; // ����Ʈ ���������̺��� ����Ʈ �ѹ��� �˻�
	UPROPERTY(VisibleAnywhere)
		TArray<int32> FinishedQuestNumbers; // ����Ʈ ���������̺��� ����Ʈ �ѹ��� �˻�

public:
	UPlayerStateSave();
	void LoadPlayerQuest(TArray<int32>* QuestNums, TArray<int32>* FinishedQuests);
	void SavePlayerQuest(TArray<int32>* QuestNums, TArray<int32>* FinishedQuests);

	void SaveSlot();
	
};
