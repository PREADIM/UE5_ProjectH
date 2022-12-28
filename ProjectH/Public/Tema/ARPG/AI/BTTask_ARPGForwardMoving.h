// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ARPGForwardMoving.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTTask_ARPGForwardMoving : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ARPGForwardMoving();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
