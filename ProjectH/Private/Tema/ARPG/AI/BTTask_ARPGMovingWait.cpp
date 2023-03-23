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

	bNotifyTick = true; // TickTask를 사용하려면 true 해야한다.
	bCreateNodeInstance = true; // BT는 같은 메모리를 공유하기때문에 CreateNodeInstance를 true 해주면 BT가 할당된 AI마다 고유의 AI노트가 생성됨.
}

EBTNodeResult::Type UBTTask_ARPGMovingWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//여기서 EnermtUnit 가져와서 Guard가 true라면 끝낼때 false 해주자.
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
	if (!bCoolTime) // False : 쿨타임 중이 아님.
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
