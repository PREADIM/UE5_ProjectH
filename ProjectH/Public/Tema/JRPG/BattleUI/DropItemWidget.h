// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "DropItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UDropItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DropCharName;

	UPROPERTY()
		class AJRPGPlayerController* OwnerController;
	/*UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* ItemAnimation;*/

public:
	void Init(FString CharName);
	//void PlayItemAnim();

	UFUNCTION(BlueprintCallable)
		void MouseOff();
	
};
