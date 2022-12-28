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

	// �� ���ڷ����ʹ� ������ �Ҽ����ٴ°� ��Ÿ���� ���ڷ����ʹ�. (�̰� true���� ����ħ)

	bool bCoolTime = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CoolTime"));

	if (!bCoolTime)
	{
		//��Ÿ���� false ��°��� ��Ÿ�� ���� �ƴ϶�� ��.
		//�׷��⶧���� ������ �ִ����� �Ǵ��ؾ���.

		bool bCanAttack = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CanAttack"));
		if (bCanAttack)
		{
			//��Ÿ�ӵ� ���Ұ� ������ �ֱ⶧���� false�� (���� x)
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		// ���� ��Ÿ����
		return true;
	}
}
