
// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTTask_ARPGMoving.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/JRPG/JRPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ARPGMoving::UBTTask_ARPGMoving()
{
	NodeName = TEXT("ARPGMoving");
}

EBTNodeResult::Type UBTTask_ARPGMoving::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	


	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn Fail");
		return EBTNodeResult::Failed;
	}


	uint8 MoveMode = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("MoveMode"));
	uint8 Temp = 0U;
	switch ((EEnermyMoveMode)MoveMode)
	{
	case EEnermyMoveMode::None:
		/*Temp = (uint8)EEnermyMoveMode::ForwardMoving;
		OwnerPawn->SetEnermyMoveMode((EEnermyMoveMode)Temp);
		_DEBUG("None");
		break;*/
	case EEnermyMoveMode::LeftMoving:
	case EEnermyMoveMode::RightMoving:
	case EEnermyMoveMode::BackMoving:
	case EEnermyMoveMode::ForwardMoving:
		Temp = (uint8)RandomMoving();
		OwnerPawn->SetEnermyMoveMode((EEnermyMoveMode)Temp);
		_DEBUG("Random");
		break;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("MoveMode"), Temp);

	return EBTNodeResult::Succeeded;
}


EEnermyMoveMode UBTTask_ARPGMoving::RandomMoving()
{
	switch (FMath::RandRange(0, 7))
	{
	case 0:
		_DEBUG("N");
		return EEnermyMoveMode::None;
	case 1:
	case 2:
		_DEBUG("L");
		return EEnermyMoveMode::LeftMoving;
	case 3:
	case 4:
		_DEBUG("R");
		return EEnermyMoveMode::RightMoving;
	case 5:
	case 6:
		_DEBUG("F");
		return EEnermyMoveMode::ForwardMoving;
	case 7:
		_DEBUG("B");
		return EEnermyMoveMode::BackMoving;
	}

	return EEnermyMoveMode::None;
}

