#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "JRPGCharStat.generated.h"



USTRUCT(BlueprintType)
struct FJRPGCharStat : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 CharLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MaxMP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Attack; // ���ݷ�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Shelid; // ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MaxULT;


public:
	FJRPGCharStat();
	FJRPGCharStat(float HP, float MP, float AT, float DFS, float ULT);
};

