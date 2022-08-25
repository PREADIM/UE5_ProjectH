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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* SkillImg;
};


USTRUCT(BlueprintType)
struct FJRPGULTSkillState
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ULTName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UseULT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* ULTImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* FillULTImg;
};

