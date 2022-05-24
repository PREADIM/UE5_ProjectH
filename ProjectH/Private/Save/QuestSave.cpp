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
	CanCnt = 0;
}

FbQuestFlag::FbQuestFlag(bool bFlag1, bool bFlag2, bool bFlag3, int32 Cnt)
{
	bSucceed = bFlag1;
	bQuesting = bFlag2;
	bCanAccept = bFlag3;
	CanCnt = Cnt;
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

void UQuestSave::SaveNPC(FString Name, FNPCAllQuest NPCQuest, bool bSucceed, bool bQuesting, bool bCanAccept, int32 QuestCnt)
{
	if (NPCQuests.Find(Name) && bNPCSucceed.Find(Name))
	{
		NPCQuests[Name] = NPCQuest;
		bNPCSucceed[Name] = FbQuestFlag(bSucceed, bQuesting, bCanAccept, QuestCnt);
	}
	else
	{
		NPCQuests.Add(Name, NPCQuest);
		bNPCSucceed.Add(Name, FbQuestFlag(bSucceed, bQuesting, bCanAccept, QuestCnt));
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
	NPC->bQuestSucceed = Flag->bSucceed;
	NPC->bQuestSucceed = Flag->bQuesting;
	NPC->bCanAccept = Flag->bCanAccept;
	NPC->CanQuestCnt = Flag->CanCnt;
	return true;
}


void UQuestSave::SaveSlot()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}

