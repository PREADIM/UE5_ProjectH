// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/SaveGame.h"
#include "QuestStruct.h"
#include "NPCAllQuest.h"
#include "QuestSave.generated.h"

USTRUCT(BlueprintType)
struct FbQuestFlag
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		bool bSucceed;
	UPROPERTY()
		bool bQuesting;
	UPROPERTY()
		bool bCanAccept;
	UPROPERTY()
		bool bCanMainAccept;
	UPROPERTY()
		int32 CanCnt;
	UPROPERTY()
		int32 MainCanCnt;

	FbQuestFlag();
	FbQuestFlag(bool bFlag_Succeed, bool bFlag_Questing, bool bFlag_CanAccept, bool bFalg_CanMainAccept, int32 Cnt_CanCnt, int32 Cnt_MainCanCnt);

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

	void SaveNPC(FString Name, FNPCAllQuest NPCQuest, bool bSucceed, bool bQuesting, bool bCanAccept, bool bMainQuest, int32 QuestCnt, int32 MainQuestCnt);
	bool LoadNPC(class AQuestNPCBase* NPC);


private :
	UPROPERTY(VisibleAnywhere)
		TArray<FQuestStruct> Quests;
	UPROPERTY(VisibleAnywhere)
		int32 CurrentQuestId;
	UPROPERTY(VisibleAnywhere)
		bool bCanChangeQuest;
	UPROPERTY(VisibleAnywhere)
		TMap<FString, FNPCAllQuest> NPCQuests;
	UPROPERTY(VisibleAnywhere)
		TMap<FString, FbQuestFlag> bNPCSucceed;




	void SaveSlot(); // 세이브 파일 저장.

};