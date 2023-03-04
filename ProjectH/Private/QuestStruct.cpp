#include "QuestStruct.h"
#include "NPCQuest.h"


FQuestStruct::FQuestStruct()
{
	QuestName = FString("");
	QuestDescription = FString("");
	QuestType = EQuestType::NONE;
	QuestSteps.Empty();
	OwnerNPCName = FString("");
	ClearNextQuest.Empty();
}



FQuestStruct::FQuestStruct(const FNPCQuest& Quest)
{
	QuestNumber = Quest.QuestNumber;
	QuestName = Quest.QuestName;
	QuestDescription = Quest.QuestDescription;
	QuestType = Quest.QuestType;
	QuestSteps = Quest.QuestSteps;
	OwnerNPCName = Quest.OwnerNPCName;
	ClearNextQuest = Quest.ClearNextQuest;
}

void FQuestStruct::Clear() // ActiveQuest�� ���� �Լ�. ��ü �ʱ�ȭ
{
	QuestName = FString("");
	QuestDescription = FString("");
	QuestType = EQuestType::NONE;
	QuestSteps.Empty();
	QuestName = FString("");
	ClearNextQuest.Empty();
}