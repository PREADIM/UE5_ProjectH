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
		int32 EnermyUnits; // 적 유닛만 사용하는 기능, 무슨 유닛을가지고 있는지 판단.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EnermyLevel;
};


// 디버프 상태
UENUM()
enum class EDebuffState : uint8
{
	DEF_DOWN UMETA(DisplayName = "DEF_DOWN"),
	STUN UMETA(DisplayName = "STUN")
};


// 디버프가 몇 카운트 남았는지 판단.
USTRUCT()
struct FDebuffStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		EDebuffState DebuffState;
	UPROPERTY(VisibleAnywhere)
		int32 cnt;
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
		float Shelid; // 방어력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxULT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NextEXP;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Priority;


public:
	FJRPGCharStat();
};



