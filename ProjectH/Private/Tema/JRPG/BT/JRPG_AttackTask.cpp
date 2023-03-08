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

	switch ((EJRPGAttackState)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(FName("ActiveType")))
	{
	case EJRPGAttackState::NormalAttack : 
		SelfUnit->NormalAttack();
		break;
	case EJRPGAttackState::SkillAttack :
		SelfUnit->Skill_1();
		break;
	case EJRPGAttackState::ULTAttack :
		SelfUnit->Skill_ULT();
		break;
	default:
		break;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsTurn"), false);

	return EBTNodeResult::Succeeded;
}