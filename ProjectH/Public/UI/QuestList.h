// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "QuestList.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UQuestList : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UQuestSlot> BP_QuestSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UActiveQuestSlot> BP_ActiveQuestSlot;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* Scroll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* CurrentQuest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* MainQuest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* OtherQuests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* MainListName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UQuestComponent* QuestComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<class UQuestSlot*> SlotArr;

	UPROPERTY()
		class UActiveQuestSlot* AQSlot;
	int32 ActiveQuestIndex = -1; // ���� ��Ƽ�� ����Ʈ�� ����Ű�� ���� �ε��� ����.

public:

	void Init();

	UFUNCTION()
		void UpdateQuestList();

	UFUNCTION()
		void CompleteStepSetSlot(int32 QuestIndex);

	void ActiveClearButtonClick(); // ��Ƽ�� ����Ʈ ������ ���������� ������ ����Ǿ��ϴ� �Լ�.
};
