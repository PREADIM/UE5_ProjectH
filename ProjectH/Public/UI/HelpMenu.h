// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "HelpMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UHelpMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* HelpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* PrevButton;

public:
	void Init();

	void HelpUIAnim(bool IsOpened);
	void HelpMenuRemove();

};
