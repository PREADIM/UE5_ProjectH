#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "JRPGCharStat.generated.h"

USTRUCT(BlueprintType)
struct FEnermys
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EnermyUnits = 0; // 적 유닛만 사용하는 기능, 무슨 유닛을가지고 있는지 판단.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EnermyLevel = 0;
};


USTRUCT(BlueprintType)
struct FJRPGCharStat : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CharLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Attack; // 공격력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Shield; // 방어력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxULT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NextEXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Priority;


public:
	FJRPGCharStat();
};



