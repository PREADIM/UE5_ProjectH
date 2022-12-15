// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "AIController.h"
#include "ARPGAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AARPGAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBlackboardComponent* BB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AARPGEnermy* OwnerUnit;

public:
	/*-------------------
		Virtual Function
	---------------------*/
	virtual void BeginPlay();
	virtual void OnPossess(APawn* InPawn);
	virtual void OnUnPossess();
	
	AARPGAIController();
};
