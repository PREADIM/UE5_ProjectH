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
		int32 EnermyUnits = 0; // �� ���ָ� ����ϴ� ���, ���� ������������ �ִ��� �Ǵ�.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EnermyLevel = 0;
};


// ����� ����
UENUM()
enum class EDebuffState : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	DEF_DOWN UMETA(DisplayName = "DEF_DOWN"),
	STUN UMETA(DisplayName = "STUN")
};


// ������� �� ī��Ʈ ���Ҵ��� �Ǵ�.
USTRUCT()
struct FDebuffStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		EDebuffState DebuffState = EDebuffState::NONE;
	UPROPERTY(VisibleAnywhere)
		int32 cnt = 0;
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
		float Attack; // ���ݷ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Shelid; // ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxULT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NextEXP;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Priority;


public:
	FJRPGCharStat();
};



