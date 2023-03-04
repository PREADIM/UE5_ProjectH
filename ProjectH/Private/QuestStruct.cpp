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

void FQuestStruct::Clear() // ActiveQuest를 위한 함수. 전체 초기화
{
	QuestName = FString("");
	QuestDescription = FString("");
	QuestType = EQuestType::NONE;
	QuestSteps.Empty();
	QuestName = FString("");
	ClearNextQuest.Empty();
}