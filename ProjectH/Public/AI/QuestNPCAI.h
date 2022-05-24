// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "AI/QuestNPCBase.h"
#include "QuestNPCAI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AQuestNPCAI : public AQuestNPCBase
{
	GENERATED_BODY()


public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Interact_Implementation(class AProjectHCharacter* OwnerCharacter) override;

private:


	
};
