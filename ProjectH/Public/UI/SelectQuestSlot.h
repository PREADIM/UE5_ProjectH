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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* QuestStateImage;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* SelectAnim;


	UPROPERTY(EditAnywhere)
		class UTexture2D* CanAcceptTex;
	UPROPERTY(EditAnywhere)
		class UTexture2D* QuestingTex;
	UPROPERTY(EditAnywhere)
		class UTexture2D* SucceedTex;

	UPROPERTY(EditAnywhere)
		FLinearColor MainQuestColor;
	UPROPERTY(EditAnywhere)
		FLinearColor SubQuestColor;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 QuestNumber;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString QuestName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 Num;
	UPROPERTY(VisibleAnywhere)
		EDialougeState DialougeState; // ���̾�α��� ���� (����Ʈ��, �Ϸ�, ��������)
	UPROPERTY(VisibleAnywhere)
		EQuestType QuestType; // ����Ʈ�� �������� �ƴ����� ����.

	UPROPERTY()
		class UDialogueWidget* Dial;

	

public:
	void Init();
	void SetupQuestStateImg();
	UFUNCTION()
		void BindQuestStartButton(); // ����Ʈ�� Ŭ���ϸ� �ش� ����Ʈ�� ���̾�αװ� ���.
};
