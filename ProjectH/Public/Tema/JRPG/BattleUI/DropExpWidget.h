// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "DropExpWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UDropExpWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DropExp;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* ExpAnimation;

public:
	void Init(float Exp);
	void PlayExpAnim();

	
};
