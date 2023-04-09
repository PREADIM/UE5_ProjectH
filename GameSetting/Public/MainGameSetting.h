// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameSetting.h"
#include "UObject/NoExportTypes.h"
#include "MainGameSetting.generated.h"

/**
 * 
 */
UCLASS(config=MainGameSetting)
class GAMESETTING_API UMainGameSetting : public UObject
{
	GENERATED_BODY()

public:
	UMainGameSetting();

	int32 GetResIndex() const { return ResIndex; }
	float GetMouseSensitivity() const { return MouseSensitivity; }
	float GetMaster() const { return MasterSound; }

public:
	UPROPERTY(Config)
		int32 ResIndex;
	UPROPERTY(Config)
		float MouseSensitivity;
	UPROPERTY(Config)
		float MasterSound;
};
