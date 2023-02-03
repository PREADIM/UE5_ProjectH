// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTTask_ARPGAttack.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ARPGAttack::UBTTask_ARPGAttack()
{
	NodeName = TEXT("ARPGAttack");
	bNotifyTick = true; // TickTask를 사용하려면 true 해야한다.
	bCreateNodeInstance = true; // BT는 같은 메모리를 공유하기때문에 CreateNodeInstance를 true 해주면 BT가 할당된 AI마다 고유의 AI노트가 생성됨.
}

EBTNodeResult::Type UBTTask_ARPGAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn Fail");
		return EBTNodeResult::Failed;
	}

	BTComp = &OwnerComp;

	if (OwnerPawn->bBlocking)
	{
		OwnerPawn->Guard(false);
	}

	if (OwnerPawn->bHitting)
		OwnerPawn->bHitting = false;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);

	OwnerPawn->OnAttack.AddDynamic(this, &UBTTask_ARPGAttack::AttackEnd);

	/*OwnerPawn->OnAttack.AddLambda([this, &OwnerComp]()->void
	{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CoolTime"), true);
			bAttack = false;
			_DEBUG("Attack End");
	});*/

	if (!bAttack)
	{
		int32 index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("AttackIndex"));
		bAttack = true;
		OwnerPawn->bMoving = false;
		OwnerPawn->Attack(index);
	}

	return EBTNodeResult::InProgress;
	//종료 지연
}


void UBTTask_ARPGAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bAttack)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}



void UBTTask_ARPGAttack::AttackEnd()
{
	BTComp->GetBlackboardComponent()->SetValueAsBool(TEXT("CoolTime"), true);
	bAttack = false;
	_DEBUG("Attack End");
}