// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "AIController.h"
#include "JRPGFieldAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AJRPGFieldAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	AJRPGFieldAIController();

	void BattleStart();
	void ReturnToField();

	UFUNCTION()
		void RestartBT();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class UBlackboardComponent* BB;
	UPROPERTY()
		class AJRPGFieldEnermy* FieldUnit;
	
};
