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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SkillDamage; // 스킬 데미지 계수.
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ULTDamage; // 스킬 데미지 계수.
};

