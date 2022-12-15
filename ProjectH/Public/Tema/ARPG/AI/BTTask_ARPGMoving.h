// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ARPGMoving.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTTask_ARPGMoving : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ARPGMoving();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
public:
	EEnermyMoveMode RandomMoving();

};


