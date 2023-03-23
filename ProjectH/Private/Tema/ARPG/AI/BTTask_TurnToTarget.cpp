// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTTask_TurnToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/ARPG/ARPGEnermy.h"


// �̷� �Լ��� ��ũ�ҿ� ���� ���ӿ��� ���⿡ ���� ���� �ұ�?

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("ARPGTrunTarget");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawn)
		return EBTNodeResult::Failed;

	AARPGUnit* Target = Cast<AARPGUnit>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetUnit")));
	if (!Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - OwnerPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	OwnerPawn->SetActorRotation(FMath::RInterpTo(OwnerPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 10.0f));

	return EBTNodeResult::Succeeded;
}
