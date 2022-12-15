// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ARPGDistance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTService_ARPGDistance : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_ARPGDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
