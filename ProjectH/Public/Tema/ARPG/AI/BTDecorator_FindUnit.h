// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_FindUnit.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTDecorator_FindUnit : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_FindUnit();


protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
