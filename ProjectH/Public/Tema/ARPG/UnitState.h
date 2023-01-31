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
	//���ݹ��� HP�� Set
	void SetTakeDamageHP(float TakeHP);
	//���ο� AP�� Set
	void SetAP(float TakeAP);

	/* ���� �⺻ ���� */
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
		float NormallyPoise; // ���ε�


	/* �ǽð� �����ϴ� ���� */
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
		float Poise; // ���ε�

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnitBase* OwnerUnit; 

};