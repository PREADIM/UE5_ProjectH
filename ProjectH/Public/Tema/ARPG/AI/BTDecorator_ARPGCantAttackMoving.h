// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ARPGCantAttackMoving.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTDecorator_ARPGCantAttackMoving : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_ARPGCantAttackMoving();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
