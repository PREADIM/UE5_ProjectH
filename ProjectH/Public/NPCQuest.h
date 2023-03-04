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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 QuestNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestName; // 퀘스트의 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestDescription; // 퀘스트의 받을때의 요약 설명.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EQuestType QuestType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FQuestStep> QuestSteps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UQuestingFunction> BP_QuestingFunction;
	UPROPERTY(VisibleAnywhere)
		class UQuestingFunction* QuestingFunction;

	// 런타임중 설정.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OwnerNPCName;
	UPROPERTY(EditAnywhere)
		TArray<int32> ClearNextQuest; // 클리어하면 다음 진행가능한 퀘스트 넘버.


	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
		bool bCanAccepted; // 인터랙트가 가능한 퀘스트. 수락가능, 완료가능, 퀘스트중 모두 해당.
	UPROPERTY(VisibleAnywhere)
		bool Questing; // 해당 퀘스트는 진행중인 퀘스트이다.
	UPROPERTY(VisibleAnywhere)
		bool CanSucceed; // 해당 퀘스트는 완료'가능'한 퀘스트이다.



	FNPCQuest();
	FNPCQuest(const FQuestStruct& Quest);
};