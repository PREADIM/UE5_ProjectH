// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTTask_ARPGTargetNot.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ARPGTargetNot::UBTTask_ARPGTargetNot()
{
	NodeName = TEXT("TargetNot");
}

EBTNodeResult::Type UBTTask_ARPGTargetNot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn Fail");
		return EBTNodeResult::Failed;
	}

	OwnerPawn->bMoving = false;
	OwnerPawn->SetEnermyMoveMode(EEnermyMoveMode::None);
	//OwnerPawn->SetBlocking(false); // ���� ���. Mini�� ���� ��ü
	OwnerPawn->SetBattleMode(false); // ��Ʋ���
	OwnerPawn->SetCollisionRadius(false);
	OwnerPawn->GetCharacterMovement()->MaxWalkSpeed = OwnerPawn->NormalSpeed;

	return EBTNodeResult::Succeeded;
}
