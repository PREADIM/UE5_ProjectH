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


	switch ((EJRPGAttackState)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(FName("ActiveType")))
	{
	case EJRPGAttackState::NormalAttack : // Normal
		SelfUnit->NormalAttack();
		break;
	case EJRPGAttackState::SkillAttack :// Skill
		SelfUnit->Skill_1();
		break;
	case EJRPGAttackState::ULTAttack : // ULT
		SelfUnit->Skill_ULT();
		break;
	default:
		break;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsTurn"), false);

	return EBTNodeResult::Succeeded;
}



//EBTNodeResult::Type UJRPG_AttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
//
//	AJRPGUnit* SelfUnit = Cast<AJRPGUnit>(OwnerComp.GetAIOwner()->GetPawn());
//
//	/*SelfUnit->OnAIAttackEnd.Clear();
//	SelfUnit->OnAIAttackEnd.AddLambda([this, &OwnerComp]()->void
//		{
//			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 4U);
//		});*/
//
//
//	switch (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(FName("ActiveType")))
//	{
//	case 1U: // Noraml
//		SelfUnit->NormalAttack();
//		break;
//	case 2U: // Skill
//		SelfUnit->Skill_1();
//		break;
//	case 3U: // ULT
//		SelfUnit->Skill_ULT();
//		break;
//	default:
//		break;
//	}
//
//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsTurn"), false);
//
//	return EBTNodeResult::Succeeded;
//}