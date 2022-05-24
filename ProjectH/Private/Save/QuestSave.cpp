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

FbQuestFlag::FbQuestFlag(bool bFlag1, bool bFlag2, bool bFlag3, bool bFalg4, int32 Cnt1, int32 Cnt2)
{
	bSucceed = bFlag1;
	bQuesting = bFlag2;
	bCanAccept = bFlag3;
	bCanMainAccept = bFalg4;
	CanCnt = Cnt1;
	MainCanCnt = Cnt2;
}


UQuestSave::UQuestSave()
{
	bCanChangeQuest = true;
}


void UQuestSave::SaveQuest(TArray<FQuestStruct> GetQuests, const int32 GetCurrentQuestId)
{
	// ���۷����� �ƴ� ���縦 �ؼ� �����Ѵ�.

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
	if (!AllQuests) // ������ ����
		return false;

	FbQuestFlag* Flag = bNPCSucceed.Find(NPC->NPCName);
	if (!Flag) // ������ ����
		return false;

	NPC->SetNPCQuests(*AllQuests); // ���������� �޴� �Լ� �̹Ƿ� ���� �״�� ����.

	// ����Ʈ ���¸� ���� ������ ��������.
	NPC->bQuestSucceed = Flag->bSucceed;
	NPC->bQuestSucceed = Flag->bQuesting;
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

