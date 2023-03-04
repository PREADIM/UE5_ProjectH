// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ProjectH.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectHGameMode.generated.h"

UCLASS(minimalapi)
class AProjectHGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectHGameMode();

	virtual void PostLogin(APlayerController* Login) override;
};



