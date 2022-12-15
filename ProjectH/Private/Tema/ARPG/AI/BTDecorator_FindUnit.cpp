// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTDecorator_FindUnit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "Tema/ARPG/ARPGEnermy.h"

UBTDecorator_FindUnit::UBTDecorator_FindUnit()
{
	NodeName = TEXT("ARPGFindUnitCanAttack");
}

bool UBTDecorator_FindUnit::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn Fail");
		return false;
	}


	AARPGUnit* Target = Cast<AARPGUnit>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetUnit")));
	if (!Target)
	{
		_DEBUG("Target Fail");
		return false;
	}

	bResult = OwnerPawn->GetDistanceTo(Target) <= OwnerPawn->GetBattleDistance();
	// 일촉즉발의 상황인가?

	OwnerPawn->SetBlocking(bResult); // 방패 들기.
	OwnerPawn->SetBattleMode(bResult); // 배틀모드
	OwnerPawn->bMoving = bResult; // 무빙 실행★

	if (bResult)
	{
		OwnerPawn->GetCharacterMovement()->MaxWalkSpeed = OwnerPawn->BattleSpeed;	
		OwnerPawn->SetBattleDistance(OwnerPawn->BattleCollisionRadius);
		//_DEBUG("Find true");
	}
	else
	{
		OwnerPawn->GetCharacterMovement()->MaxWalkSpeed = OwnerPawn->NormalSpeed;
		OwnerPawn->SetEnermyMoveMode(EEnermyMoveMode::None);
		OwnerPawn->SetBattleDistance(OwnerPawn->NormalDistance);
		//_DEBUG("Find false");
	}

	
	return bResult;
}
