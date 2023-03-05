#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "JRPGCharList.generated.h"



USTRUCT(BlueprintType)
struct FJRPGCharList : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CharNum = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AJRPGUnit> BP_JRPGCharacter;
};
