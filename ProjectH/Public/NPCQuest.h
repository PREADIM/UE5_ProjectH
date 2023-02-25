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


	// ��Ÿ���� ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OwnerNPCName;
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
		bool bCanAccepted; // ���ͷ�Ʈ�� ������ ����Ʈ

	UPROPERTY(VisibleAnywhere)
		bool Questing; // �ش� ����Ʈ�� �������� ����Ʈ�̴�.
	UPROPERTY(VisibleAnywhere)
		bool CanSucceed; // �ش� ����Ʈ�� �Ϸ�'����'�� ����Ʈ�̴�.

	FNPCQuest();
	FNPCQuest(const FQuestStruct& Quest);

};