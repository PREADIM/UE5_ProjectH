// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "JRPG_TargetLook.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UJRPG_TargetLook : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UJRPG_TargetLook();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
