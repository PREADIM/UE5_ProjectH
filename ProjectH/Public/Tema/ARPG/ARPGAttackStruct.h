#pragma once

#include "ProjectH.h"
#include "ARPGAttackStruct.generated.h"


USTRUCT(BlueprintType)
struct FARPGAttackStruct
{
	GENERATED_USTRUCT_BODY()

public:
	// BT���� �̰͵�� �Ǵ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanThisAttack = true; // ���� ��� �������� �ǽð����� üũ.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackDistance; // ������ ����. �ش� ������ ���ؼ� �� �켱���� ������ ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 CurrentCnt; // ���� �� ��ų�� ����� Ƚ��.
	

	FARPGAttackStruct();
	FARPGAttackStruct(float Distance);


};


