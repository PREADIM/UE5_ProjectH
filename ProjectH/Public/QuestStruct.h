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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AQuestNPCBase* OwnerNPC; // °¡´É?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bSucceed = false;

	FQuestStruct();
	FQuestStruct(const FNPCQuest& Quest);

	void Clear();

};