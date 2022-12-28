// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ARPGCoolTime.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTService_ARPGCoolTime : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_ARPGCoolTime();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


public:
	float AttackCoolTime; // �� ��ü�� ���� ��Ÿ��

	float CoolTime;
	bool bCoolTime; // ��Ÿ���� ��������?
};
