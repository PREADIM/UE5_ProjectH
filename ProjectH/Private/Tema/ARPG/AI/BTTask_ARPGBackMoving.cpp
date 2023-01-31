// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTTask_ARPGBackMoving.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ARPGBackMoving::UBTTask_ARPGBackMoving()
{
	NodeName = TEXT("ARPGBackMoving");
}

EBTNodeResult::Type UBTTask_ARPGBackMoving::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("CurrentUnit")));
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn Fail");
		return EBTNodeResult::Failed;
	}

	uint8 BackMove = (uint8)EEnermyMoveMode::BackMoving;
	OwnerPawn->SetEnermyMoveMode(EEnermyMoveMode::BackMoving);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("MoveMode"), BackMove);

	return EBTNodeResult::Succeeded;
}
