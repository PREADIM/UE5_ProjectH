#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "QuestStep.h"
#include "QuestStruct.h"
#include "NPCQuest.generated.h"



USTRUCT(BlueprintType)
struct FNPCQuest
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
	/* 퀘스트가 진행중 일때, 말을 걸어서 퀘스트를 진행하는 경우 (맵 이동 등) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UQuestingFunction> BP_QuestingFunction;
	UPROPERTY(VisibleAnywhere)
		class UQuestingFunction* QuestingFunction;

	/* 해당 퀘스트의 NPC 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OwnerNPCName;
	/* 이 퀘스트 클리어 후 진행 가능해지는 퀘스트 번호  */
	UPROPERTY(EditAnywhere)
		TArray<int32> ClearNextQuest;

	/* 인터랙트가 가능한 퀘스트. 수락가능, 완료가능, 퀘스트중 모두 해당. */
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
		bool bCanAccepted;
	UPROPERTY(VisibleAnywhere)
		bool Questing; // 진행중인 퀘스트 판별
	UPROPERTY(VisibleAnywhere)
		bool CanSucceed; // 완료'가능'한 퀘스트 판별



	FNPCQuest();
	FNPCQuest(const FQuestStruct& Quest);
};