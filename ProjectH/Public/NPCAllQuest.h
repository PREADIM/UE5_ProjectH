#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "NPCQuest.h"
#include "NPCAllQuest.generated.h"


USTRUCT(BlueprintType)
struct FNPCAllQuest
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FNPCQuest> Quests;
};



