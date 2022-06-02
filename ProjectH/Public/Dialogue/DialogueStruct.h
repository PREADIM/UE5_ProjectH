#pragma once

#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "Dialogue/TextNAnim.h"
#include "DialogueStruct.generated.h"

USTRUCT(BlueprintType)
struct FDialogueStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 QuestNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTextNAnim> Dialogue;
};