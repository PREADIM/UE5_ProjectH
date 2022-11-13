// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BT/JRPG_ActiveTask.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGAIController.h"



UJRPG_ActiveTask::UJRPG_ActiveTask()
{
	NodeName = TEXT("JRPG_ActiveTask");
}

EBTNodeResult::Type UJRPG_ActiveTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AJRPGGameMode* GM = Cast<AJRPGGameMode>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GM")));

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 0U); // NONE

	if (!GM)
	{
		return EBTNodeResult::Failed;
	}


	AJRPGUnit* AIUnit = Cast<AJRPGUnit>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AIUnit)
	{
		return EBTNodeResult::Failed;
	}


	TArray<AJRPGUnit*> OwnerList = GM->OwnerList;
	AJRPGUnit* TargetAttackUnit = nullptr;


	for (AJRPGUnit* Unit : OwnerList)
	{
		// 여기서 if문으로 도발 스킬중인 유닛이있으면 바로 해당 유닛으로 하고 break 하면 될듯.
	
		if (TargetAttackUnit == nullptr)
		{
			TargetAttackUnit = Unit;
		}
		else
		{
			TargetAttackUnit = TargetAttackUnit->CurrentHP > Unit->CurrentHP ? TargetAttackUnit : Unit;
		}
	}



	if (AIUnit->ULTGage >= AIUnit->MaxULTGage)
	{
		if (AIUnit->CurrentMP >= AIUnit->UnitSkills.Skill_1.CostMP)
		{
			float DFEDamage = 100 / (100 + TargetAttackUnit->CharacterStat.Shelid);
			float SkillDamage = AIUnit->UnitSkills.Skill_1.SkillDamage * AIUnit->CharacterStat.Attack;
			float Damage = SkillDamage * DFEDamage;
			if (TargetAttackUnit->CurrentHP <= Damage)
			{
				// 열거형으로 무슨 상태가 가능한지 선택한다.
				// 스킬을 쓸수있는 상태
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 2U);
			}
			else
			{
				// 궁극기 상태
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 3U);
			}
		}
		else
		{
			// 궁극기 상태
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 3U);
		}
	}
	else if (AIUnit->CurrentMP >= AIUnit->UnitSkills.Skill_1.CostMP)
	{
		// 스킬 상태
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 2U);
	}
	else
	{
		// 일반 공격 상태
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 1U);	
	}
	

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), TargetAttackUnit);

	return EBTNodeResult::Succeeded;
}
