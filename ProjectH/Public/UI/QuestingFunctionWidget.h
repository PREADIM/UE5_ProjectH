// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainQuestUI.h"
#include "QuestingFunctionWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UQuestingFunctionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* SlotButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* QuestingFlagText;
	UPROPERTY()
		class UDialogueWidget* Dial;
	UPROPERTY()
		class UMainQuestUI* OwnerMainWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		FString FlagText;

	int32 Index;

public:
	void Init(class UQuestingFunction* QuestingFunction, int32 FlagIndex);
};
