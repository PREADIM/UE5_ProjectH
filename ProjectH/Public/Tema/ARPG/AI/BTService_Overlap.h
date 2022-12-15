// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Overlap.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTService_Overlap : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_Overlap();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
