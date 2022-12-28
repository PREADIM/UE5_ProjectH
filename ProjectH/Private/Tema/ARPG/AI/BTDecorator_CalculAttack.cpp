// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTDecorator_CalculAttack.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/JRPG/JRPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tema/ARPG/ARPGAttackComponent.h"
#include "Tema/ARPG/AttackClass.h"


UBTDecorator_CalculAttack::UBTDecorator_CalculAttack()
{
	NodeName = TEXT("CalculAttack");
}


bool UBTDecorator_CalculAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn false");
		return false;
	}
	

	APawn* TargetUnit = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetUnit")));
	if (!TargetUnit)
	{
		_DEBUG("TargetUnit false");
		return false;
	}


	//true면 쿨타임 중 이라는 뜻.
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CoolTime")))
	{
		return false;
	}

	const TArray<UAttackClass*>& Attacks = OwnerPawn->GetAttackComponent()->GetAttacks();

	float AttackDistance = OwnerPawn->GetDistanceTo(TargetUnit);
	float CurrentAttackDistance = -100.0f; // 현재 어택의 거리를 비교해서 가장 거리가 먼 공격을 사용
	bool bCanAttack = false;
	int32 AttackIndex = -1;

	if (AttackDistance >= OwnerPawn->GetAttackComponent()->MaxAttackDistance + 70.f)
	{
		// 현재 거리가 어택의 최대 길이보다 큰 경우 애초에 할 필요 x
		return false;
	}

	for (int32 i = 0; i < Attacks.Num(); i++)
	{
		//스킬 사용가능한지.
		if (Attacks[i]->AttackStruct.bCanThisAttack)
		{
			// 현재 거리보다 이 스킬을 사용가능한 거리가 더 작은경우 이 스킬을 최우선으로 사용
			if (AttackDistance >= Attacks[i]->AttackStruct.AttackDistance)
			{
				// 이 if문을 또 하는이유는, 공격 범위에 닿지않는 곳인데도 불구하고 공격을 할 수도 있기때문.
				if (AttackDistance <= Attacks[i]->AttackStruct.AttackDistance + 70.f)
				{
					float Dist = Attacks[i]->AttackStruct.AttackDistance;
					if (CurrentAttackDistance < Dist)
					{
						AttackIndex = i;
						CurrentAttackDistance = Dist;
						bCanAttack = true;
						//_DEBUG("%d Attack true", i);
					}
				}
				//else if (Attacks[i]->AttackStruct.AttackDistance == 0.f)
				//{
				//	// 기본 공격의 경우
				//	if (CurrentAttackDistance == -1.f)
				//	{
				//		AttackIndex = i;
				//		bCanAttack = true;
				//		//_DEBUG("Normal Attack true");
				//	}
				//}
			}
			//이 스킬의 범위는 현재 거리보다 작거나 같다.
		}
	}


	if (AttackIndex == -1)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);
		_DEBUG("Attack Index -1");
		return false;
	}


	OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("AttackIndex"), AttackIndex);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), bCanAttack);

	if (bCanAttack)
		return true;
	else
		return false;
}