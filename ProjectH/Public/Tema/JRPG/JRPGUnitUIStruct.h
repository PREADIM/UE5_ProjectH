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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* PartySettingIcon;
	
	FJRPGUnitUIStruct();

};


USTRUCT()
struct FAttackNameAndDetail
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString AttackName;
	UPROPERTY(EditAnywhere)
		FString AttackDetail;

	FAttackNameAndDetail() {}
};

/* 배틀 때의 스킬 설명 */
USTRUCT(BlueprintType)
struct FJRPGUnitAttackDetail : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		FAttackNameAndDetail NormalAttackDetail;
	UPROPERTY(EditAnywhere)
		FAttackNameAndDetail SkillAttackDetail;
	UPROPERTY(EditAnywhere)
		FAttackNameAndDetail ULTAttackDetail;
};

