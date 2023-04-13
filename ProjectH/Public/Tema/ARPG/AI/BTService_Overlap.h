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

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreActors;

	bool bFindUnit = false;
	
	UPROPERTY(EditAnywhere)
		float FindUnitSecond = 0.f; /* 유닛을 놓쳤을때 몇초동안은 유닛을 무조건 따라갈 것인지. */
	float CurrentSecond = 0.f;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
