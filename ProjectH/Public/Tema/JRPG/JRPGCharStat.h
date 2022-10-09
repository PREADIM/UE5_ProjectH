#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "JRPGCharStat.generated.h"



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
		float Shelid; // 방어력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxULT;


public:
	FJRPGCharStat();
	FJRPGCharStat(float HP, float MP, float AT, float DFS, float ULT);
};

