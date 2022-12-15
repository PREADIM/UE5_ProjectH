// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ARPGBackMoving.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTTask_ARPGBackMoving : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ARPGBackMoving();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
