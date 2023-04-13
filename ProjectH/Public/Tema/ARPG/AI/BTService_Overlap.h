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
		float FindUnitSecond = 0.f; /* ������ �������� ���ʵ����� ������ ������ ���� ������. */
	float CurrentSecond = 0.f;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
