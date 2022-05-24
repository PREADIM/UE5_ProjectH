// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "QTESlotUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UQTESlotUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/*-----------------
		Public Value
	-------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* KeyText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* TimeBar;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Success;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Fail;


	UPROPERTY(EditAnywhere)
		FKey Key;
	UPROPERTY(EditAnywhere)
		float TotalTime;
	UPROPERTY(EditAnywhere)
		float PressedTime;

protected:
	virtual void NativeConstruct() override;


public:
	/*------------------
		Public Function
	--------------------*/
	void UpdateTime(float PressTime);
	float PlayWidgetAnim(bool bFlag);

};
