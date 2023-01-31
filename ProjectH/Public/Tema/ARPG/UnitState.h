#pragma once
#include "ProjectH.h"
#include "UnitState.generated.h"



USTRUCT(BlueprintType)
struct FUnitState
{
	GENERATED_USTRUCT_BODY()

public:
	FUnitState();

	void Init(class AARPGUnitBase* Unit);
	//공격받은 HP로 Set
	void SetTakeDamageHP(float TakeHP);
	//새로운 AP로 Set
	void SetAP(float TakeAP);

	/* 평상시 기본 스텟 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormallyHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormallyMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormallyAP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormallyATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormallyDEF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormallyPoise; // 강인도


	/* 실시간 변경하는 스텟 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float HP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float AP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float ATK;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float DEF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Poise; // 강인도

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnitBase* OwnerUnit; 

};