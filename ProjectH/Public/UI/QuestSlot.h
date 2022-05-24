// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "QuestSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UQuestSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	/*-----------------
		Public Value
	-------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* Canvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ColorPen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* QuestNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* QuestDescriptionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* QuestFollow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UQuestComponent* QuestComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString QuestName;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor SelectColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor NormalColor;


public:
	/*---------------
		public Func
	----------------*/

	UFUNCTION()
		void Init();

	UFUNCTION()
		void BindQuestDescription();

	UFUNCTION()
		void ActiveQuestColor();

	UFUNCTION()
		void ButtonClicked();

	UFUNCTION(BlueprintImplementableEvent)
		void SetOutline(bool bActvie);

	void SetColorPenSize(); /* 글자 크기에 따라서 이미지 크기 조정 하기. */

	void SetColorPen();
	
};
