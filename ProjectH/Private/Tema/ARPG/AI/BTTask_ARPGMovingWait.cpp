// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTTask_ARPGMovingWait.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/ARPGAIController.h"

UBTTask_ARPGMovingWait::UBTTask_ARPGMovingWait()
{
	NodeName = TEXT("MovingWait");
	WaitTime = 2.f;
	RandomTime = 0.f;
	CurrentTime = 0.0f;
	RandomWaitTime = 0.0f;

	bNotifyTick = true; // TickTask�� ����Ϸ��� true �ؾ��Ѵ�.
	bCreateNodeInstance = true; // BT�� ���� �޸𸮸� �����ϱ⶧���� CreateNodeInstance�� true ���ָ� BT�� �Ҵ�� AI���� ������ AI��Ʈ�� ������.
}

EBTNodeResult::Type UBTTask_ARPGMovingWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//���⼭ EnermtUnit �����ͼ� Guard�� true��� ������ false ������.
	OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("CurrentUnit")));
	if (!OwnerPawn)
		return EBTNodeResult::Failed;

	CurrentTime = 0.f;
	RandomWaitTime = FMath::RandRange(0.f, RandomTime) + WaitTime;

	return EBTNodeResult::InProgress;
}

void UBTTask_ARPGMovingWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	bCoolTime = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CoolTime"));
	if (!bCoolTime) // False : ��Ÿ�� ���� �ƴ�.
	{
		bCanAttack = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CanAttack"));
		if (bCanAttack)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

	CurrentTime += DeltaSeconds;
	if(CurrentTime >= RandomWaitTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
