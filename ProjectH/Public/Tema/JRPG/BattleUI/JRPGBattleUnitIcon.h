// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "Tema/JRPG/DebuffClass.h"
#include "JRPGBattleUnitIcon.generated.h"

/**
 * 
 */
UCLASS(Meta = (DisalbeNativeTick))
class PROJECTH_API UJRPGBattleUnitIcon : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UImage* CharImg;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CharText;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* BuffIconWrapBox;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* CurrentUnit;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* InitIcon;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UJRPGBuffWidget> BP_BuffWidget;
	UPROPERTY()
		TArray<class UJRPGBuffWidget*> BuffWidgets;

public:
	void SetDebuffIcon(TSet<FDebuffStruct> DebuffSet);
	void Init(class UTexture2D* CharTex, FString CharName);
	void PlayInitAnim();
	void PlayCurrentAnim();
	
};
