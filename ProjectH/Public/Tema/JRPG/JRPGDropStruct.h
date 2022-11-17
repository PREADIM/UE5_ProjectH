#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "JRPGDropStruct.generated.h"


USTRUCT(BlueprintType)
struct FJRPGDropStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DropExp; // ������ ���� ����ġ.


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DropCharNum;
	//���⼭ �Ը��� �°� ������ �������� �ۼ��ϸ� �ɵ�.

public:
	FJRPGDropStruct();

};

