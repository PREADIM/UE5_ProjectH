#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "QuestDataBase.generated.h"


USTRUCT(BlueprintType)
struct FQuestDataBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 QuestNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString NPCName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description;
};

