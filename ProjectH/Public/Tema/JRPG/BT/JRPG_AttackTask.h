// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTTaskNode.h"
#include "JRPG_AttackTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UJRPG_AttackTask : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
