#include "QuestStruct.h"
#include "NPCQuest.h"


FQuestStruct::FQuestStruct()
{
	QuestName = FString("");
	QuestDescription = FString("");
	QuestType = EQuestType::NONE;
	QuestSteps.Empty();
	OwnerNPC = nullptr;
	bSucceed = false;
}



FQuestStruct::FQuestStruct(const FNPCQuest& Quest)
{
	QuestNumber = Quest.QuestNumber;
	QuestName = Quest.QuestName;
	QuestDescription = Quest.QuestDescription;
	QuestType = Quest.QuestType;
	QuestSteps = Quest.QuestSteps;
	OwnerNPC = Quest.OwnerNPC;
	bSucceed = false;
}

void FQuestStruct::Clear() // ActiveQuest�� ���� �Լ�. ��ü �ʱ�ȭ
{
	/*QuestName.Empty();
	QuestDescription.Empty();
	QuestSteps.Empty();*/
	QuestName = FString("");
	QuestDescription = FString("");
	QuestType = EQuestType::NONE;
	QuestSteps.Empty();
	OwnerNPC = nullptr;
	bSucceed = false;
}