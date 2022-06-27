#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "BattleFieldList.generated.h"



USTRUCT(BlueprintType)
struct FBattleFieldList : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FieldNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString FieldName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector SpawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABattleField> BP_BattleField;
};