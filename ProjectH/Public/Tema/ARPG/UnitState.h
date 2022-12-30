#pragma once
#include "ProjectH.h"
#include "UnitState.generated.h"



USTRUCT(BlueprintType)
struct FUnitState
{
	GENERATED_USTRUCT_BODY()

public:
	FUnitState();
	void SetTakeDamageHP(float TakeDamageHP);
	//���ݹ��� HP�� Set

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DEF;

};