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
	// ��Ƽ�� ����Ʈ�� �����Ҷ� for������ �������� ���°� ���� �ش� ����Ʈ �ε����� ã�Ƽ� �� �ι����� ���� �ٲٱ⸦ ���� ����

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

	void SetColorPenSize(); /* ���� ũ�⿡ ���� �̹��� ũ�� ���� �ϱ�. */
	void SetColorPen();
};
