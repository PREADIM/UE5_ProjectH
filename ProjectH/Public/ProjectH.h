// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"


#define SCREEN_DEBUG(Time,Color,Format,...)		GEngine->AddOnScreenDebugMessage(-1, Time, Color, FString::Printf(TEXT(Format), ##__VA_ARGS__));
#define _DEBUG(Format, ...)						GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__));
#define SET_OPTION_S(Format, path, ...)			GConfig->SetString(TEXT("/Script/GameSetting.MainGameSetting"), TEXT(Format), ##__VA_ARGS__, path)
#define SET_OPTION(Format, path, ...)			GConfig->SetInt(TEXT("/Script/GameSetting.MainGameSetting"), TEXT(Format), ##__VA_ARGS__, path)


const FString RES_COMMAND = "r.SetRes ";
const FString AA_COMMAND = "r.PostProcessAAQuality ";
const FString S_COMMAND = "sg.ShadowQuality ";
const FString T_COMMAND = "sg.TextureQuality ";