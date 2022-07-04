#pragma once
#include "ProjectH.h"
#include "JRPGSkillState.generated.h"


USTRUCT(BlueprintType)
struct FJRPGSkillState
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CostMP;
};

