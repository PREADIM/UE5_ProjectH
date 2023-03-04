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
		FString QuestName; // ����Ʈ�� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestDescription; // ����Ʈ�� �������� ��� ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EQuestType QuestType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FQuestStep> QuestSteps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UQuestingFunction> BP_QuestingFunction;
	UPROPERTY(VisibleAnywhere)
		class UQuestingFunction* QuestingFunction;

	// ��Ÿ���� ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OwnerNPCName;
	UPROPERTY(EditAnywhere)
		TArray<int32> ClearNextQuest; // Ŭ�����ϸ� ���� ���డ���� ����Ʈ �ѹ�.


	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
		bool bCanAccepted; // ���ͷ�Ʈ�� ������ ����Ʈ. ��������, �Ϸᰡ��, ����Ʈ�� ��� �ش�.
	UPROPERTY(VisibleAnywhere)
		bool Questing; // �ش� ����Ʈ�� �������� ����Ʈ�̴�.
	UPROPERTY(VisibleAnywhere)
		bool CanSucceed; // �ش� ����Ʈ�� �Ϸ�'����'�� ����Ʈ�̴�.



	FNPCQuest();
	FNPCQuest(const FQuestStruct& Quest);
};