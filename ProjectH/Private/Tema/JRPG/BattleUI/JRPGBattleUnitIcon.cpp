// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGBattleUnitIcon.h"
#include "Components/Image.h"
#include "COmponents/TextBlock.h"

void UJRPGBattleUnitIcon::Init(class UTexture2D* CharTex, FString CharName)
{
	CharImg->SetBrushFromTexture(CharTex);
	CharText->SetText(FText::FromString(CharName));
	PlayInitAnim();
}

void UJRPGBattleUnitIcon::PlayInitAnim()
{
	PlayAnimation(InitIcon, 0.0f, 1, EUMGSequencePlayMode::Forward);
}

void UJRPGBattleUnitIcon::PlayCurrentAnim()
{
	PlayAnimation(CurrentUnit, 0.0f, 1, EUMGSequencePlayMode::Forward);
}

