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
		float DropExp; // 죽으면 나올 경험치.

	//여기서 입맛에 맞게 떨구는 아이템을 작성하면 될듯.

public:
	FJRPGDropStruct() {}

};

