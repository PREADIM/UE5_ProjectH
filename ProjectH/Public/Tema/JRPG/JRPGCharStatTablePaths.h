#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "JRPGCharStatTablePaths.generated.h"

// 위의 스텟 테이블을 Path로 한 데이터테이블에 모아놓고 일일히 코드에 추가하지않아도 데이터테이블에 추가하면 알아서
// 자동으로 추가하도록 설정하기위한 구조체

USTRUCT(BlueprintType)
struct FTablePath : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Path;

};


USTRUCT(BlueprintType)
struct FJRPGCharStatTablePaths : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CharNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTablePath TablePath;

public:
	FJRPGCharStatTablePaths();

};

