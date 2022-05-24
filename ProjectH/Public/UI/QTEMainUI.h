// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "Special/QTE/QTEWidgetPosition.h"
#include "QTEMainUI.generated.h"

/**
 *
 */
UCLASS()
class PROJECTH_API UQTEMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	/*-----------------
		Public Value
	-------------------*/
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCanvasPanel* Canvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UQTESlotUI> BP_QTEWidget;
	UPROPERTY()
		class UQTESlotUI* QTEWidget;


public:
	/*------------------
		Public Function
	--------------------*/
	class UQTESlotUI* AddQTEWidget(FKey Key, float TotalTime, FQTEWidgetPosition ScreenPosition);

};
