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

	
	// 아직 공격 타이밍이 아닌 경우 시간을 잰다.
	bCoolTime = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CoolTime"));

	//쿨타임이 true 인경우 (쿨타임 중 이라는 뜻)
	if (bCoolTime)
	{
		AttackCoolTime = OwnerPawn->GetAttackCoolTime();
		CoolTime += DeltaSeconds;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);

		// 쿨타임이므로 당연히 때릴 수 없음.
		if (AttackCoolTime <= CoolTime)
		{
			CoolTime = 0.f;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CoolTime"), false);
		}
	}
	else // 쿨타임이 아닌경우 공격 가능한지 계산
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
			// 현재 거리가 어택의 최대 길이보다 큰 경우 애초에 할 필요 x
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);
			return;
		}

		const TArray<UAttackClass*>& Attacks = OwnerPawn->GetAttackComponent()->GetAttacks();
		float CurrentAttackDistance = OwnerPawn->GetAttackComponent()->MaxAttackDistance + 100.f; // 현재 어택의 거리를 비교해서 가장 거리가 먼 공격을 사용
		bool bCanAttack = false;
		int32 AttackIndex = -1;

		for (int32 i = 0; i < Attacks.Num(); i++)
		{
			//스킬 사용가능한지.
			if (Attacks[i]->AttackStruct.bCanThisAttack)
			{
				// 현재 거리보다 이 스킬을 사용가능한 거리가 더 작은경우 이 스킬을 최우선으로 사용
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
				//이 스킬의 범위는 현재 거리보다 작거나 같다.
			}
		}

		// 거리가 되는 공격이 없는 경우
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

