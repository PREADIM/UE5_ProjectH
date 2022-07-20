#pragma once

#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "JRPGUnitUIStruct.generated.h"


USTRUCT(BlueprintType)
struct FJRPGUnitUIStruct : public FTableRowBase
{

	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CharacterNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* CharTex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CharName;



	FJRPGUnitUIStruct();

};

