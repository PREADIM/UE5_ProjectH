#pragma once

#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "Dialogue/TextNAnim.h"
#include "DialogueStruct.generated.h"


UENUM(BlueprintType)
enum class EDialougeState : uint8
{
	CanSucceed UMETA(DisplayName = "SucceedQuest"),
	Questing UMETA(DisplayName = "Questing"),
	CanAccept UMETA(DisplayName = "CanAccept")
};

USTRUCT(BlueprintType)
struct FTextNNameState
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTextNName> Dialogues;	
};

USTRUCT(BlueprintType)
struct FDialogueStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		int32 QuestNumber = 0;

	UPROPERTY(EditAnywhere)
		TMap<EDialougeState, FTextNNameState> DialogueMap;

	UPROPERTY(EditAnywhere)
		FString Description; // 알아보기위한 설명서 
};



USTRUCT(BlueprintType)
struct FQuestCinematic : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		int32 QuestNumber = 0;

	UPROPERTY(EditAnywhere)
		class ULevelSequence* QuestSequence = nullptr;
};