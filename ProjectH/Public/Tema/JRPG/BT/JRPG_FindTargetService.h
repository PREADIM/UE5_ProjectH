// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTService.h"
#include "JRPG_FindTargetService.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UJRPG_FindTargetService : public UBTService
{
	GENERATED_BODY()

public:
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<class AActor*> IgnoreActors;

	UJRPG_FindTargetService();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
