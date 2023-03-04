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
	QuestingNums = QuestingQuests; // �Ϸ��� ����Ʈ�� �ϳ��� �ִ°�?
	SucceedQuestNums = SucceedQuests;
	EndedQuestsNums = EndedQuests;
}


UQuestSave::UQuestSave()
{
}


void UQuestSave::SaveQuest(TArray<FQuestStruct> GetQuests, const int32 GetCurrentQuestId)
{
	// ���۷����� �ƴ� ���縦 �ؼ� �����Ѵ�.

	_DEBUG("SaveQuest");
	Quests = GetQuests;
	CurrentQuestId = GetCurrentQuestId;
	SaveSlot();
}



void UQuestSave::LoadQuest(class UQuestComponent* QuestComponent)
{
	// ĳ���Ϳ� ������ ���� �ִ°� �ƴ϶�, �ε�� ����Ʈ�� AddQuest�� Ʈ���Ÿ� �����Ѵ�.
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
	if (!NPC_QSQ) // ������ ����
		return false;

	// ����Ʈ ���¸� ���� ������ ��������.
	NPC->QuestingNums = NPC_QSQ->QuestingNums; // NPC���� ������ �ϳ��� �Ϸ��� ���� �ִ���
	NPC->SucceedQuestsNums = NPC_QSQ->SucceedQuestNums; // ����Ʈ ������
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

