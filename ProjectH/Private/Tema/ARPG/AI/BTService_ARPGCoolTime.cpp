// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTService_ARPGCoolTime.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tema/ARPG/ARPGAttackComponent.h"
#include "Tema/ARPG/AttackClass.h"

UBTService_ARPGCoolTime::UBTService_ARPGCoolTime()
{
	NodeName = TEXT("AttackCoolTime");

	CoolTime = 0.0f;
}

void UBTService_ARPGCoolTime::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn Fail");
		return;
	}

	
	// ���� ���� Ÿ�̹��� �ƴ� ��� �ð��� ���.
	bCoolTime = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CoolTime"));

	//��Ÿ���� true �ΰ�� (��Ÿ�� �� �̶�� ��)
	if (bCoolTime)
	{
		AttackCoolTime = OwnerPawn->GetAttackCoolTime();
		CoolTime += DeltaSeconds;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);

		// ��Ÿ���̹Ƿ� �翬�� ���� �� ����.
		if (AttackCoolTime <= CoolTime)
		{
			CoolTime = 0.f;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CoolTime"), false);
		}
	}
	else // ��Ÿ���� �ƴѰ�� ���� �������� ���
	{
		APawn* TargetUnit = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetUnit")));
		if (!TargetUnit)
		{
			_DEBUG("TargetUnit false");
			return;
		}


		float AttackDistance = OwnerPawn->GetDistanceTo(TargetUnit);

		if (AttackDistance > OwnerPawn->GetAttackComponent()->MaxAttackDistance)
		{
			// ���� �Ÿ��� ������ �ִ� ���̺��� ū ��� ���ʿ� �� �ʿ� x
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);
			return;
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
					}
					else if (CurrentAttackDistance == Dist)
					{
						if (FMath::RandRange(1, 10) % 2 == 0)
						{
							AttackIndex = i;
							CurrentAttackDistance = Dist;
							bCanAttack = true;
						}
					}
				}
				//�� ��ų�� ������ ���� �Ÿ����� �۰ų� ����.
			}
		}

		// �Ÿ��� �Ǵ� ������ ���� ���
		if (AttackIndex == -1)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);
			_DEBUG("Attack Index -1");
			return;
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("AttackIndex"), AttackIndex);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), bCanAttack);
	}

}

