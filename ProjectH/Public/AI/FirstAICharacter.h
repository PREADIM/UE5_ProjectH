// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "AI/ProjectHAICharacter.h"
#include "FirstAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AFirstAICharacter : public AProjectHAICharacter
{
	GENERATED_BODY()

public:
	AFirstAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;


private:



};
