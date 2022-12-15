// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTService_ARPGDistance.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/JRPG/JRPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_ARPGDistance::UBTService_ARPGDistance()
{
	NodeName = TEXT("Distance");
}

void UBTService_ARPGDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn Fail");
		return;
	}

	AARPGUnit* TargetUnit = Cast<AARPGUnit>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetUnit")));
	if (!TargetUnit)
		return;

	bool bAttackDistance = OwnerPawn->GetDistanceTo(TargetUnit) <= OwnerPawn->GetAttackDistance();
	//���� Ÿ����ġ���� ���ݹ����� �����ϰų� ����?
	// true ���� / false ����
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), bAttackDistance);

}
