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
		FString QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EQuestType QuestType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FQuestStep> QuestSteps;


	// 런타임중 설정.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OwnerNPCName;
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
		bool bCanAccepted; // 인터랙트가 가능한 퀘스트

	UPROPERTY(VisibleAnywhere)
		bool Questing; // 해당 퀘스트는 진행중인 퀘스트이다.
	UPROPERTY(VisibleAnywhere)
		bool CanSucceed; // 해당 퀘스트는 완료'가능'한 퀘스트이다.

	FNPCQuest();
	FNPCQuest(const FQuestStruct& Quest);

};