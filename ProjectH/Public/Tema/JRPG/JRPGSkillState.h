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
		float CostMP = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* SkillImg = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SkillDamage = 0.f; // 스킬 데미지 계수.
};


USTRUCT(BlueprintType)
struct FJRPGULTSkillState
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ULTName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UseULT = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* ULTImg = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* FillULTImg = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ULTDamage = 0.f; // 스킬 데미지 계수.
};

