
// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTTask_ARPGMoving.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ARPGMoving::UBTTask_ARPGMoving()
{
	NodeName = TEXT("ARPGMoving");
}

EBTNodeResult::Type UBTTask_ARPGMoving::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("CurrentUnit")));
	if (!OwnerPawn)
	{
		return EBTNodeResult::Failed;
	}


	TargetUnit = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetUnit")));
	if (!TargetUnit)
	{
		return EBTNodeResult::Failed;
	}


	uint8 MoveMode = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("MoveMode"));
	uint8 Temp = 0U;
	switch ((EEnermyMoveMode)MoveMode)
	{
	case EEnermyMoveMode::None:
		Temp = (uint8)EEnermyMoveMode::ForwardMoving;
		OwnerPawn->SetEnermyMoveMode((EEnermyMoveMode)Temp);
		break;
	case EEnermyMoveMode::LeftMoving:
	case EEnermyMoveMode::RightMoving:
	case EEnermyMoveMode::BackMoving:
	case EEnermyMoveMode::ForwardMoving:
		Temp = (uint8)RandomMoving();
		OwnerPawn->SetEnermyMoveMode((EEnermyMoveMode)Temp);
		break;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("MoveMode"), Temp);

	return EBTNodeResult::Succeeded;
}


EEnermyMoveMode UBTTask_ARPGMoving::RandomMoving()
{
	int32 StartRange = 0;
	if (OwnerPawn->GetDistanceTo(TargetUnit) <= 70.f)
	{
		StartRange = 2;
	}

	switch (FMath::RandRange(StartRange, 6))
	{
	case 0:
	case 1:	
		return EEnermyMoveMode::ForwardMoving;
	case 2:
	case 3:
		return EEnermyMoveMode::LeftMoving;	
	case 4:
	case 5:
		return EEnermyMoveMode::RightMoving;
	case 6:
		return EEnermyMoveMode::BackMoving;
	}

	return EEnermyMoveMode::None;
}

