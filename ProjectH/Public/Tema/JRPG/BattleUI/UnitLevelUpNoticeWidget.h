// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "UnitLevelUpNoticeWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UUnitLevelUpNoticeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Notice;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LevelNotice;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* NoticeAnimation;

public:
	void Init(FString Name, int32 Level);
	void PlayExpAnim();

};
