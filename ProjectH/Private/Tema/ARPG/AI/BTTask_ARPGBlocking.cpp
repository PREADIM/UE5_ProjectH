// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTTask_ARPGBlocking.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_ARPGBlocking::UBTTask_ARPGBlocking()
{
	NodeName = TEXT("ARPG Blocking");
}

EBTNodeResult::Type UBTTask_ARPGBlocking::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn false");
		return EBTNodeResult::Failed;
	}


	switch (FMath::RandRange(0, 1))
	{
	case 0:
		OwnerPawn->Guard(false);
		return EBTNodeResult::Succeeded;
	case 1:
		OwnerPawn->Guard(true);
		return EBTNodeResult::Succeeded;	
	}

	return EBTNodeResult::Succeeded;
}
