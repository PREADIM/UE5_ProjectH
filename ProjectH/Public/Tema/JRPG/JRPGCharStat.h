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
		float Attack; // ���ݷ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Shelid; // ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxULT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EXP;


public:
	FJRPGCharStat();
	FJRPGCharStat(float HP, float MP, float AT, float DFS, float ULT);
};



// ���� ���� ���̺��� Path�� �� ���������̺� ��Ƴ��� ������ �ڵ忡 �߰������ʾƵ� ���������̺� �߰��ϸ� �˾Ƽ�
// �ڵ����� �߰��ϵ��� �����ϱ����� ����ü
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