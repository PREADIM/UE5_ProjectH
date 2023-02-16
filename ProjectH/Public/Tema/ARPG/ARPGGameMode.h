// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/GameModeBase.h"
#include "ARPGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
	AARPGGameMode();
	virtual void PostLogin(APlayerController* Login) override;
	

public:
	void Restart();
};
