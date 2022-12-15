// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTTask_ARPGAttack.h"
#include "Tema/JRPG/JRPGAIController.h"
#include "Tema/ARPG/ARPGEnermy.h"

UBTTask_ARPGAttack::UBTTask_ARPGAttack()
{
	NodeName = TEXT("ARPGAttack");
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


	OwnerPawn->OnAttack.AddLambda([this]()->void 
	{
			bAttack = false;
	});

	if (!bAttack)
	{
		bAttack = true;
		OwnerPawn->Attack();		
	}

	_DEBUG("AI Attack");

	return EBTNodeResult::InProgress;
	//종료 지연
}


void UBTTask_ARPGAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeletaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeletaSeconds);

	if (!bAttack)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}