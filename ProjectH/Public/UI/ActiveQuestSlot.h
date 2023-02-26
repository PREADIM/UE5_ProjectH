// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "ActiveQuestSlot.generated.h"

/**
 *
 */
UCLASS()
class PROJECTH_API UActiveQuestSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	/*-----------------
		Public Value
	-------------------*/
	UPROPERTY()
		class UQuestList* OwnerList; 
	// 액티브 퀘스트를 해제할때 for문으로 전역으로 도는것 보다 해당 퀘스트 인덱스를 찾아서 단 두번만에 색상 바꾸기를 위한 오너

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* Canvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ColorPen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* QuestNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* QuestDescriptionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* QuestClearButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UQuestComponent* QuestComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString QuestName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor SelectColor;

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
		void ClearButtonClicked();

	UFUNCTION(BlueprintImplementableEvent)
		void SetOutline(bool bActvie);

	void SetColorPenSize(); /* 글자 크기에 따라서 이미지 크기 조정 하기. */
	void SetColorPen();
};
