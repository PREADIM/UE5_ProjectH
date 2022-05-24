// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "ProjectH.h"
#include "GameFramework/HUD.h"
#include "ProjectHHUD.generated.h"

UCLASS()
class AProjectHHUD : public AHUD
{
	GENERATED_BODY()

public:
	AProjectHHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

