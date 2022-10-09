// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BT/JRPG_AttackTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGAIController.h"



EBTNodeResult::Type UJRPG_AttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AJRPGUnit* SelfUnit = Cast<AJRPGUnit>(OwnerComp.GetAIOwner()->GetPawn());

	/*SelfUnit->OnAIAttackEnd.Clear();
	SelfUnit->OnAIAttackEnd.AddLambda([this, &OwnerComp]()->void
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 4U);
		});*/


	switch (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(FName("ActiveType")))
	{
	case 1U: // Nroaml
		SelfUnit->NormalAttack();
		_DEBUG("Call Normal");
		break;
	case 2U: // Skill
		SelfUnit->Skill_1();
		_DEBUG("Call Skill");
		break;
	case 3U: // ULT
		SelfUnit->Skill_ULT();
		_DEBUG("Call ULT");
		break;
	default:
		break;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsTurn"), false);

	return EBTNodeResult::Succeeded;
}



/*void UJRPG_AttackTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (4U == OwnerComp.GetBlackboardComponent()->GetValueAsEnum(FName("ActiveType")))
	{
		_DEBUG("Finish");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}*/