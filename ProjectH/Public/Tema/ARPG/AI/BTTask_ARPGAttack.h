// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ARPGAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTTask_ARPGAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ARPGAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeletaSeconds) override;

public:
	class AARPGEnermy* OwnerPawn;
	bool bAttack;
};
