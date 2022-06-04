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

	FString GetResolution() const { return Resolution; }
	int32 GetAA() const { return AASetting; }
	int32 GetShadowQ() const { return TextureQ; }
	int32 GetTextureQ() const { return ShadowQ; }
	float GetMouseSensitivity() const { return MouseSensitivity; }

public:
	UPROPERTY(Config)
		FString Resolution;
	UPROPERTY(Config)
		int32 AASetting;
	UPROPERTY(Config)
		int32 ShadowQ;
	UPROPERTY(Config)
		int32 TextureQ;
	UPROPERTY(Config)
		float MouseSensitivity;
	
	/*UPROPERTY(Config)
		int temp;
	UPROPERTY(Config)
		int num;*/


};
