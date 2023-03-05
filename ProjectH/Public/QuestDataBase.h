#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "NPCAllQuest.h"
#include "QuestDataBase.generated.h"


USTRUCT(BlueprintType)
struct FQuestDataBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 QuestNumber = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString NPCName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description;
};


// NPC들에게는 퀘스트 넘버를 가지고있고, 해당 퀘스트 넘버를 통해 검색하여 NPC가져오기.
USTRUCT(BlueprintType)
struct FNPCQuestDataBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString NPCName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FNPCAllQuest NPCAllQuests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description;
};

