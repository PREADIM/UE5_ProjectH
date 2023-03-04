// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/SaveGame.h"
#include "QuestStruct.h"
#include "NPCAllQuest.h"
#include "QuestSave.generated.h"

USTRUCT(BlueprintType)
struct FNPCQuestingAndSucceedQuests
{
	GENERATED_USTRUCT_BODY()

public:

	//UPROPERTY를 넣지않으면 Save Load가 안됨.
	UPROPERTY()
		TSet<int32> QuestingNums;
	UPROPERTY()
		TSet<int32> SucceedQuestNums;
	UPROPERTY()
		TSet<int32> EndedQuestsNums;

	FNPCQuestingAndSucceedQuests();
	FNPCQuestingAndSucceedQuests(TSet<int32> QuestingQuests, TSet<int32> SucceedQuests, TSet<int32> EndedQuests);
};
/**
 * 
 */
UCLASS()
class PROJECTH_API UQuestSave : public USaveGame
{
	GENERATED_BODY()


public:
	UQuestSave();

	static const FString SlotName;


	void SaveQuest(TArray<FQuestStruct> GetQuests, const int32 GetCurrentQuestId);
	void LoadQuest(class UQuestComponent* QuestComponent);

	void SaveNPC(FString Name, TSet<int32> QuestingQuests, TSet<int32> SucceedQuests, TSet<int32> EndedQuests);
	bool LoadNPC(class AQuestNPCBase* NPC);

	FQuestStruct* GetQuests(int32 QuestNumber);
	void SaveSlot(); // 세이브 파일 저장.

private :
	UPROPERTY(VisibleAnywhere)
		TArray<FQuestStruct> Quests;
	UPROPERTY(VisibleAnywhere)
		int32 CurrentQuestId;

	UPROPERTY(VisibleAnywhere)
		TMap<FString, FNPCAllQuest> NPCQuests;
	UPROPERTY(VisibleAnywhere)
		TMap<FString, FNPCQuestingAndSucceedQuests> NPCQuestingAndSucceedQuest;
};