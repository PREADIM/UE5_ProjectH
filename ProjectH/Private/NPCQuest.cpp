#include "NPCQuest.h"
#include "QuestStruct.h"

FNPCQuest::FNPCQuest() : bCanAccepted(false)
{
}


FNPCQuest::FNPCQuest(const FQuestStruct& Quest)
{
	QuestNumber = Quest.QuestNumber;
	QuestName = Quest.QuestName;
	QuestDescription = Quest.QuestDescription;
	QuestType = Quest.QuestType;
	QuestSteps = Quest.QuestSteps;
	OwnerNPC = Quest.OwnerNPC;
	bCanAccepted = false;

	//bPrimary = Quest.bPrimary;
	//bCanAccepted = false;

}