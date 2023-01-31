// Fill out your copyright notice in the Description page of Project Settings.

#include "Tema/ARPG/AI/BTTask_ARPGForwardMoving.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ARPGForwardMoving::UBTTask_ARPGForwardMoving()
{
	NodeName = TEXT("ARPGForwardMoving");
}

EBTNodeResult::Type UBTTask_ARPGForwardMoving::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("CurrentUnit")));
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn Fail");
		return EBTNodeResult::Failed;
	}

	uint8 ForwardMove = (uint8)EEnermyMoveMode::ForwardMoving;
	OwnerPawn->SetEnermyMoveMode(EEnermyMoveMode::ForwardMoving);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("MoveMode"), ForwardMove);

	return EBTNodeResult::Succeeded;
}