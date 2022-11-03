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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EXP;


public:
	FJRPGCharStat();
	FJRPGCharStat(float HP, float MP, float AT, float DFS, float ULT);
};



// 위의 스텟 테이블을 Path로 한 데이터테이블에 모아놓고 일일히 코드에 추가하지않아도 데이터테이블에 추가하면 알아서
// 자동으로 추가하도록 설정하기위한 구조체
USTRUCT(BlueprintType)
struct FJRPGCharStatTablePaths : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CharNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Path;

};