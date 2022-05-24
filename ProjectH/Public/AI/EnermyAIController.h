// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "AI/ProjectHAIController.h"
#include "EnermyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AEnermyAIController : public AProjectHAIController
{
	GENERATED_BODY()

public:
	AEnermyAIController();

public:
	virtual void BeginPlay() override;



public:
	/*-------------------
		Virtual Function
	---------------------*/
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
};
