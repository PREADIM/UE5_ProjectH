#pragma once

#include "ProjectH.h"
#include "ARPGAttackStruct.generated.h"


USTRUCT(BlueprintType)
struct FARPGAttackStruct
{
	GENERATED_USTRUCT_BODY()

public:
	// BT에서 이것들로 판단.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanThisAttack = true; // 현재 사용 가능한지 실시간으로 체크.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackDistance; // 공격의 범위. 해당 범위를 비교해서 뭘 우선으로 쓸건지 결정.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamageFactor; // 이 공격의 데미지 계수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AP_DMG; // 이 공격이 AP를 얼마나 깎는지 (AP가 0이면 방패가 깨진다.)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 CurrentCnt = 0; // 현재 이 스킬을 사용한 횟수.
	

	FARPGAttackStruct();
	FARPGAttackStruct(float Distance);


};


