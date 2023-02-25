// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "UI/DialogueWidget.h"
#include "NPCQuest.h"
#include "SelectQuestSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API USelectQuestSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* QuestNameText;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* SelectAnim;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 QuestNumber;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString QuestName; // ����

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 Num;

	UPROPERTY(VisibleAnywhere)
		EDialougeState DialougeState;

	UPROPERTY()
		class UDialogueWidget* Dial;

	

public:
	void Init();
	UFUNCTION()
		void BindQuestStartButton(); // ����Ʈ�� Ŭ���ϸ� �ش� ����Ʈ�� ���̾�αװ� ���.
};
