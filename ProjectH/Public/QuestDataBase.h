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


// NPC�鿡�Դ� ����Ʈ �ѹ��� �������ְ�, �ش� ����Ʈ �ѹ��� ���� �˻��Ͽ� NPC��������.
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

