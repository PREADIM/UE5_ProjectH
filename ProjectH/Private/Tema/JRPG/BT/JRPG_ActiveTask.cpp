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
		_DEBUG("Not BT GM");
		return EBTNodeResult::Failed;
	}


	AJRPGUnit* AIUnit = Cast<AJRPGUnit>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AIUnit)
	{
		_DEBUG("Not AIUnit");
		return EBTNodeResult::Failed;
	}


	TArray<AJRPGUnit*> OwnerList = GM->OwnerList;
	AJRPGUnit* TargetAttackUnit = nullptr;


	for (AJRPGUnit* Unit : OwnerList)
	{
		// ���⼭ if������ ���� ��ų���� ������������ �ٷ� �ش� �������� �ϰ� break �ϸ� �ɵ�.
	
		if (TargetAttackUnit == nullptr)
		{
			TargetAttackUnit = Unit;
		}
		else
		{
			TargetAttackUnit = TargetAttackUnit->CurrentHP < Unit->CurrentHP ? TargetAttackUnit : Unit;
		}
	}



	if (AIUnit->ULTGage >= AIUnit->MaxULTGage)
	{
		if (AIUnit->CurrentMP >= AIUnit->UnitSkills.Skill_1.CostMP)
		{
			float Damage = AIUnit->UnitSkills.Skill_1.SkillDamage * AIUnit->CharacterStat.Attack - TargetAttackUnit->CharacterStat.Shelid;
			if (TargetAttackUnit->CurrentHP <= Damage)
			{
				// ���������� ���� ���°� �������� �����Ѵ�.
				// ��ų�� �����ִ� ����
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 2U);
				_DEBUG("Skill_1");
			}
			else
			{
				// �ñر� ����
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 3U);
				_DEBUG("ULT");
			}
		}
		else
		{
			// �ñر� ����
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 3U);
			_DEBUG("ULT");
		}
	}
	else if (AIUnit->CurrentMP >= AIUnit->UnitSkills.Skill_1.CostMP)
	{
		// ��ų ����
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 2U);
		_DEBUG("Skill_2");
	}
	else
	{
		// �Ϲ� ���� ����
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("ActiveType"), 1U);	
		_DEBUG("Normal");
	}
	

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), TargetAttackUnit);

	return EBTNodeResult::Succeeded;
}
