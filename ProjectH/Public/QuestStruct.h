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
	/* �ش� ����Ʈ�� NPC �̸� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OwnerNPCName;
	/* �� ����Ʈ Ŭ���� �� ���� ���������� ����Ʈ ��ȣ  */
	UPROPERTY(EditAnywhere)
		TArray<int32> ClearNextQuest;

	FQuestStruct();
	FQuestStruct(const FNPCQuest& Quest);
	void Clear();
};