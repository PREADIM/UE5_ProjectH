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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamageFactor; // �� ������ ������ ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AP_DMG; // �� ������ AP�� �󸶳� ����� (AP�� 0�̸� ���а� ������.)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 CurrentCnt = 0; // ���� �� ��ų�� ����� Ƚ��.
	

	FARPGAttackStruct();
	FARPGAttackStruct(float Distance);


};


