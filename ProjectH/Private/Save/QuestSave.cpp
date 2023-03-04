// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/QuestSave.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "AI/QuestNPCBase.h"

const FString UQuestSave::SlotName = FString("QuestSave");


FNPCQuestingAndSucceedQuests::FNPCQuestingAndSucceedQuests()
{
	QuestingNums.IsEmpty();
	SucceedQuestNums.IsEmpty();
	EndedQuestsNums.IsEmpty();
}

FNPCQuestingAndSucceedQuests::FNPCQuestingAndSucceedQuests(TSet<int32> QuestingQuests, TSet<int32> SucceedQuests, TSet<int32> EndedQuests)
{
	QuestingNums = QuestingQuests; // 완료한 퀘스트가 하나라도 있는가?
	SucceedQuestNums = SucceedQuests;
	EndedQuestsNums = EndedQuests;
}


UQuestSave::UQuestSave()
{
}


void UQuestSave::SaveQuest(TArray<FQuestStruct> GetQuests, const int32 GetCurrentQuestId)
{
	// 레퍼런스가 아닌 복사를 해서 저장한다.

	_DEBUG("SaveQuest");
	Quests = GetQuests;
	CurrentQuestId = GetCurrentQuestId;
	SaveSlot();
}



void UQuestSave::LoadQuest(class UQuestComponent* QuestComponent)
{
	// 캐릭터에 생으로 갔다 넣는게 아니라, 로드된 퀘스트를 AddQuest로 트리거를 스폰한다.
	for (FQuestStruct& Quest : Quests)
	{
		QuestComponent->AddQuest(FNPCQuest(Quest));
	}

	if (Quests.Num())
		QuestComponent->SelectQuest(Quests[CurrentQuestId].QuestName);
}

void UQuestSave::SaveNPC(FString Name, TSet<int32> QuestingQuests, TSet<int32> SucceedQuests, TSet<int32> EndedQuests)
{
	if (NPCQuestingAndSucceedQuest.Find(Name))
		NPCQuestingAndSucceedQuest[Name] = FNPCQuestingAndSucceedQuests(QuestingQuests, SucceedQuests, EndedQuests);
	else
	{
		NPCQuestingAndSucceedQuest.Add(Name, FNPCQuestingAndSucceedQuests(QuestingQuests, SucceedQuests, EndedQuests));
	}

	SaveSlot();
}

bool UQuestSave::LoadNPC(AQuestNPCBase* NPC)
{
	FNPCQuestingAndSucceedQuests* NPC_QSQ = NPCQuestingAndSucceedQuest.Find(NPC->NPCName);
	if (!NPC_QSQ) // 없으면 리턴
		return false;

	// 퀘스트 상태를 위한 변수들 가져오기.
	NPC->QuestingNums = NPC_QSQ->QuestingNums; // NPC에게 유저가 하나라도 완료한 퀘가 있는지
	NPC->SucceedQuestsNums = NPC_QSQ->SucceedQuestNums; // 퀘스트 중인지
	NPC->EndedQuestsNums = NPC_QSQ->EndedQuestsNums; 

	return true;
}

FQuestStruct* UQuestSave::GetQuests(int32 QuestNumber)
{
	for (int32 i = 0; Quests.Num(); ++i)
	{
		if (Quests[i].QuestNumber == QuestNumber)
		{
			return &Quests[i];
		}
	}

	return nullptr;
}


void UQuestSave::SaveSlot()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}

