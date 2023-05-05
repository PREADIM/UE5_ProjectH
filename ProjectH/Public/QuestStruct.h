#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "QuestStep.h"
#include "QuestStruct.generated.h"


struct FNPCQuest;

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	NONE	UMETA(DisplayName = "NONE"),
	Main	UMETA(DisplayName = "MainQuest"),
	Normal	UMETA(DisplayName = "NormalQuest")
};


USTRUCT(BlueprintType)
struct FQuestStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	/* 퀘스트 번호 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 QuestNumber;
	/* 퀘스트 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestName;
	/* 퀘스트 설명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestDescription;
	/* 퀘스트 타입 (메인, 서브) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EQuestType QuestType;
	/* 퀘스트의 진행라인 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FQuestStep> QuestSteps;
	/* 해당 퀘스트의 NPC 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OwnerNPCName;
	/* 이 퀘스트 클리어 후 진행 가능해지는 퀘스트 번호  */
	UPROPERTY(EditAnywhere)
		TArray<int32> ClearNextQuest;

	FQuestStruct();
	FQuestStruct(const FNPCQuest& Quest);
	void Clear();
};