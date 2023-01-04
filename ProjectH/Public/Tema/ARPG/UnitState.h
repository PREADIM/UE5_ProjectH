#pragma once
#include "ProjectH.h"
#include "UnitState.generated.h"



USTRUCT(BlueprintType)
struct FUnitState
{
	GENERATED_USTRUCT_BODY()

public:
	FUnitState();
	//공격받은 HP로 Set
	void SetTakeDamageHP(float TakeDamageHP);
	//공격받은 AP로 Set
	void SetTakeDamageAP(float TakeDamageAP);

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