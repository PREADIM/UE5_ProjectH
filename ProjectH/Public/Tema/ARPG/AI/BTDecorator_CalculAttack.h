// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CalculAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTDecorator_CalculAttack : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CalculAttack();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
