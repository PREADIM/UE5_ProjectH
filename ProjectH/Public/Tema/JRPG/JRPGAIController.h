// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "AIController.h"
#include "JRPGAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AJRPGAIController : public AAIController
{
	GENERATED_BODY()

public:

	AJRPGAIController();

public:
	/*-------------------
		Virtual Function
	---------------------*/
	virtual void BeginPlay();
	virtual void OnPossess(APawn* InPawn);
	virtual void OnUnPossess();

	void SetIsTurn(bool bFlag);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class UBlackboardComponent* BB;
	UPROPERTY()
		class AJRPGUnit* OwnerCharacter;
	
};
