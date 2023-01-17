// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ARPGMovingWait.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTTask_ARPGMovingWait : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ARPGMovingWait();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WaitTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RandomTime;

	float CurrentTime;
	float RandomWaitTime;

	bool bCoolTime;
	bool bCanAttack;

	UPROPERTY()
		class AARPGEnermy* OwnerPawn;
};
