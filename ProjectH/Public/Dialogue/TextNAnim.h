#pragma once

#include "ProjectH.h"
#include "TextNAnim.generated.h"

USTRUCT(BlueprintType)
struct FTextNName
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText NPCName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Dialogue;
};