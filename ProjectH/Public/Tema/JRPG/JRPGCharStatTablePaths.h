#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "JRPGCharStatTablePaths.generated.h"

// ���� ���� ���̺��� Path�� �� ���������̺� ��Ƴ��� ������ �ڵ忡 �߰������ʾƵ� ���������̺� �߰��ϸ� �˾Ƽ�
// �ڵ����� �߰��ϵ��� �����ϱ����� ����ü

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

