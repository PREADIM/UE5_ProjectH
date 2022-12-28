// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTService_ARPGCoolTime.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/JRPG/JRPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	
	// ������ ���� Ÿ�̹��̰ų� ���� ���� Ÿ�̹��� �ƴ� ��� �ð��� ���.
	bCoolTime = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CoolTime"));

	//��Ÿ���� true �ΰ�� (��Ÿ�� �� �̶�� ��)
	if (bCoolTime)
	{
		AttackCoolTime = OwnerPawn->GetAttackCoolTime();
		CoolTime += DeltaSeconds;
		_DEBUG("%f", CoolTime);
		if (AttackCoolTime <= CoolTime)
		{
			bCoolTime = false;
			CoolTime = 0.f;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CoolTime"), false);
		}
	}
}

