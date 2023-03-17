// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BT/JRPG_TargetLook.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGAIController.h"
#include "Kismet/KismetMathLibrary.h"


UJRPG_TargetLook::UJRPG_TargetLook()
{
	NodeName = TEXT("JRPG_TargetLook");
}



EBTNodeResult::Type UJRPG_TargetLook::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AJRPGGameMode* GM = Cast<AJRPGGameMode>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GM")));
	if (!GM)
		return EBTNodeResult::Failed;

	AJRPGPlayerController* OwnerController = GM->OwnerController;
	if (!OwnerController)
		return EBTNodeResult::Failed;

	AJRPGUnit* TargetUnit = Cast<AJRPGUnit>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetUnit")));
	AJRPGUnit* SelfUnit = Cast<AJRPGUnit>(OwnerComp.GetAIOwner()->GetPawn());

	OwnerController->EnermyUnitTurnToTarget();

	/*FVector TargetLocation = TargetUnit->GetActorLocation();
	FVector SelfLocation = SelfUnit->GetActorLocation();
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(TargetLocation, SelfLocation);
	FRotator EnermyRot = UKismetMathLibrary::FindLookAtRotation(SelfLocation, TargetLocation);

	OwnerController->CameraSetUp(TargetLocation);
	OwnerController->CameraRotSetUp(Rot);
	
	SelfUnit->SetActorRotation(EnermyRot);*/


	return EBTNodeResult::Succeeded;
}