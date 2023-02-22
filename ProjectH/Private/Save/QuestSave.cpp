// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/QuestSave.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "AI/QuestNPCBase.h"

const FString UQuestSave::SlotName = FString("QuestSave");


FbQuestFlag::FbQuestFlag()
{
	bSucceed = false;
	bQuesting = false;
	bCanAccept = false;
	bCanMainAccept = false;
	CanCnt = 0;
	MainCanCnt = 0;
}

FbQuestFlag::FbQuestFlag(bool bFlag_Succeed, bool bFlag_Questing, bool bFlag_CanAccept, bool bFalg_CanMainAccept, int32 Cnt_CanCnt, int32 Cnt_MainCanCnt)
{
	bSucceed = bFlag_Succeed;
	bQuesting = bFlag_Questing;
	bCanAccept = bFlag_CanAccept;
	bCanMainAccept = bFalg_CanMainAccept;
	CanCnt = Cnt_CanCnt;
	MainCanCnt = Cnt_MainCanCnt;
}


UQuestSave::UQuestSave()
{
	bCanChangeQuest = true;
}


void UQuestSave::SaveQuest(TArray<FQuestStruct> GetQuests, const int32 GetCurrentQuestId)
{
	// 레퍼런스가 아닌 복사를 해서 저장한다.

	Quests = GetQuests;
	CurrentQuestId = GetCurrentQuestId;

	SaveSlot();
}



void UQuestSave::LoadQuest(class UQuestComponent* QuestComponent)
{
	for (FQuestStruct Quest : Quests)
	{
		QuestComponent->AddQuest(FNPCQuest(Quest));
	}

	if (Quests.Num())
		QuestComponent->SelectQuest(Quests[CurrentQuestId].QuestName);

	QuestComponent->UpdateQuestSave();

}

void UQuestSave::SaveNPC(FString Name, FNPCAllQuest NPCQuest, bool bSucceed, bool bQuesting, bool bCanAccept, bool bMainQuest, int32 QuestCnt, int32 MainQuestCnt)
{
	if (NPCQuests.Find(Name) && bNPCSucceed.Find(Name))
	{
		NPCQuests[Name] = NPCQuest;
		bNPCSucceed[Name] = FbQuestFlag(bSucceed, bQuesting, bCanAccept, bMainQuest, QuestCnt, MainQuestCnt);
	}
	else
	{
		NPCQuests.Add(Name, NPCQuest);
		bNPCSucceed.Add(Name, FbQuestFlag(bSucceed, bQuesting, bCanAccept, bMainQuest, QuestCnt, MainQuestCnt));
	}

	//SaveSlot();
}

bool UQuestSave::LoadNPC(AQuestNPCBase* NPC)
{
	if (!NPCQuests.Num())
		return false;

	FNPCAllQuest* AllQuests = NPCQuests.Find(NPC->NPCName);
	if (!AllQuests) // 없으면 리턴
		return false;

	FbQuestFlag* Flag = bNPCSucceed.Find(NPC->NPCName);
	if (!Flag) // 없으면 리턴
		return false;

	NPC->SetNPCQuests(*AllQuests); // 참조값으로 받는 함수 이므로 값을 그대로 전달.

	// 퀘스트 상태를 위한 변수들 가져오기.
	NPC->bQuestSucceed = Flag->bSucceed;
	NPC->bIsQuesting = Flag->bQuesting;
	NPC->bCanAccept = Flag->bCanAccept;
	NPC->bHaveMainQuest = Flag->bCanMainAccept;
	NPC->CanQuestCnt = Flag->CanCnt;
	NPC->CanMainQuestCnt = Flag->MainCanCnt;

	return true;
}


void UQuestSave::SaveSlot()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}

