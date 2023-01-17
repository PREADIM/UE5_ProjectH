// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ARPGBlocking.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTTask_ARPGBlocking : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ARPGBlocking();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
