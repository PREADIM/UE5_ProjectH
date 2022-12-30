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


	//true�� ��Ÿ�� �� �̶�� ��.
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CoolTime")))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);
		return false;
	}

	
	float AttackDistance = OwnerPawn->GetDistanceTo(TargetUnit);

	if (AttackDistance > OwnerPawn->GetAttackComponent()->MaxAttackDistance)
	{
		// ���� �Ÿ��� ������ �ִ� ���̺��� ū ��� ���ʿ� �� �ʿ� x
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);
		return false;
	}

	const TArray<UAttackClass*>& Attacks = OwnerPawn->GetAttackComponent()->GetAttacks();
	float CurrentAttackDistance = OwnerPawn->GetAttackComponent()->MaxAttackDistance + 100.f; // ���� ������ �Ÿ��� ���ؼ� ���� �Ÿ��� �� ������ ���
	bool bCanAttack = false;
	int32 AttackIndex = -1;

	for (int32 i = 0; i < Attacks.Num(); i++)
	{
		//��ų ��밡������.
		if (Attacks[i]->AttackStruct.bCanThisAttack)
		{
			// ���� �Ÿ����� �� ��ų�� ��밡���� �Ÿ��� �� ������� �� ��ų�� �ֿ켱���� ���
			if (AttackDistance <= Attacks[i]->AttackStruct.AttackDistance)
			{
				float Dist = Attacks[i]->AttackStruct.AttackDistance;
				if (CurrentAttackDistance > Dist)
				{
					AttackIndex = i;
					CurrentAttackDistance = Dist;
					bCanAttack = true;
					//_DEBUG("%d Attack true", i);
				}
			}
			//�� ��ų�� ������ ���� �Ÿ����� �۰ų� ����.
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