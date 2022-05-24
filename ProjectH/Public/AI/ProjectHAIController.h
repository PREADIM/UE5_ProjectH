// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "AIController.h"
#include "ProjectHAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AProjectHAIController : public AAIController
{
	GENERATED_BODY()

public:
	AProjectHAIController();



public:
	virtual void BeginPlay();



public:
	/*-------------------
		Virtual Function
	---------------------*/
	virtual void OnPossess(APawn* InPawn);
	virtual void OnUnPossess();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class UBlackboardComponent* BB;
	UPROPERTY()
		class AProjectHAICharacter* OwnerAICharacter;

private:



};
