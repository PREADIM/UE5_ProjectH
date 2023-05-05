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
	/* ����Ʈ ��ȣ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 QuestNumber;
	/* ����Ʈ �̸� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestName;
	/* ����Ʈ ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestDescription;
	/* ����Ʈ Ÿ�� (����, ����) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EQuestType QuestType;
	/* ����Ʈ�� ������� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FQuestStep> QuestSteps;
	/* ����Ʈ�� ������ �϶�, ���� �ɾ ����Ʈ�� �����ϴ� ��� (�� �̵� ��) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UQuestingFunction> BP_QuestingFunction;
	UPROPERTY(VisibleAnywhere)
		class UQuestingFunction* QuestingFunction;

	/* �ش� ����Ʈ�� NPC �̸� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OwnerNPCName;
	/* �� ����Ʈ Ŭ���� �� ���� ���������� ����Ʈ ��ȣ  */
	UPROPERTY(EditAnywhere)
		TArray<int32> ClearNextQuest;

	/* ���ͷ�Ʈ�� ������ ����Ʈ. ��������, �Ϸᰡ��, ����Ʈ�� ��� �ش�. */
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
		bool bCanAccepted;
	UPROPERTY(VisibleAnywhere)
		bool Questing; // �������� ����Ʈ �Ǻ�
	UPROPERTY(VisibleAnywhere)
		bool CanSucceed; // �Ϸ�'����'�� ����Ʈ �Ǻ�



	FNPCQuest();
	FNPCQuest(const FQuestStruct& Quest);
};