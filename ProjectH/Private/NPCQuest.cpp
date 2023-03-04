#include "NPCQuest.h"

FNPCQuest::FNPCQuest() : bCanAccepted(false)
{
	ClearNextQuest.Empty();
}


FNPCQuest::FNPCQuest(const FQuestStruct& Quest)
{
	QuestNumber = Quest.QuestNumber;
	QuestName = Quest.QuestName;
	QuestDescription = Quest.QuestDescription;
	QuestType = Quest.QuestType;
	QuestSteps = Quest.QuestSteps;
	OwnerNPCName = Quest.OwnerNPCName;
	bCanAccepted = false;

	ClearNextQuest = Quest.ClearNextQuest;
}