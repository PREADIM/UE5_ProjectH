// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "JRPGBattleUnitIcon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UJRPGBattleUnitIcon : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UImage* CharImg;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CharText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* CurrentUnit;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* InitIcon;

public:
	void Init(class UTexture2D* CharTex, FString CharName);
	void PlayInitAnim();
	void PlayCurrentAnim();
	
};
