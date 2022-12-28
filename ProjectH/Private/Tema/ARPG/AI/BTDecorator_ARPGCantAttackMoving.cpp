// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTDecorator_ARPGCantAttackMoving.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_ARPGCantAttackMoving::UBTDecorator_ARPGCantAttackMoving()
{
	NodeName = TEXT("Can'tAttackMoving");
}

bool UBTDecorator_ARPGCantAttackMoving::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// 이 데코레이터는 공격을 할수없다는걸 나타내는 데코레이터다. (이게 true여야 무빙침)

	bool bCoolTime = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CoolTime"));

	if (!bCoolTime)
	{
		//쿨타임이 false 라는것은 쿨타임 중이 아니라는 것.
		//그렇기때문에 때릴수 있는지도 판단해야함.

		bool bCanAttack = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CanAttack"));
		if (bCanAttack)
		{
			//쿨타임도 돌았고 때릴수 있기때문에 false다 (무빙 x)
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		// 아직 쿨타임중
		return true;
	}
}
