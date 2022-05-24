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
		TArray<int32> QuestNumbers; // 퀘스트 데이터테이블에서 퀘스트 넘버로 검색
	UPROPERTY(VisibleAnywhere)
		TArray<int32> FinishedQuestNumbers; // 퀘스트 데이터테이블에서 퀘스트 넘버로 검색

public:
	UPlayerStateSave();
	void LoadPlayerQuest(TArray<int32>* QuestNums, TArray<int32>* FinishedQuests);
	void SavePlayerQuest(TArray<int32>* QuestNums, TArray<int32>* FinishedQuests);

	void SaveSlot();
	
};
