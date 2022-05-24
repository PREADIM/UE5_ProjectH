#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "QuestStep.h"
#include "NPCQuest.generated.h"


struct FQuestStruct;


USTRUCT(BlueprintType)
struct FNPCQuest
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 QuestNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EQuestType QuestType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FQuestStep> QuestSteps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AQuestNPCBase* OwnerNPC; // °¡´É?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanAccepted; 

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPrimary = true;*/


	FNPCQuest();
	FNPCQuest(const FQuestStruct& Quest);

};