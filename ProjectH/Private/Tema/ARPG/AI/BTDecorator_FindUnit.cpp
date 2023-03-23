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

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("CurrentUnit")));
	if (!OwnerPawn)
		return false;


	AARPGUnit* Target = Cast<AARPGUnit>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetUnit")));
	if (!Target)
		return false;

	bResult = OwnerPawn->GetDistanceTo(Target) <= OwnerPawn->GetBattleDistance();

	if (bResult)
		OwnerPawn->GetCharacterMovement()->MaxWalkSpeed = OwnerPawn->BattleSpeed;	
	else
	{
		OwnerPawn->GetCharacterMovement()->MaxWalkSpeed = OwnerPawn->NormalSpeed;
		OwnerPawn->SetEnermyMoveMode(EEnermyMoveMode::None);
	}

	
	return bResult;
}
