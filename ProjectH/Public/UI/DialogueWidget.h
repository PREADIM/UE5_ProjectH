// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "Dialogue/DialogueStruct.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* NameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DialText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Exit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* SelectBox;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AProjectH_PC* OwnerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class USelectQuestSlot> BP_SelectQuestSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UQuestingFunctionWidget> BP_QuestingFunctionWidget;

	UPROPERTY()
		class AQuestNPCBase* OwnerNPC;
	int32 OwnerQuestNum = 0;

	UPROPERTY()
		class UMainQuestUI* OwnerMainWidget;

	UPROPERTY()
		TArray<FTextNName> NormalDialogue;
	int32 DialIndex = 0;
	
	UPROPERTY()
		TArray<FTextNName> SelectText;
	int32 SelectTextIndex = 0;




public:
	void NPCDialogue();
	void SetNormalDialText(int32 index);
	void SetCanQuestList();
	void SetQuestingSelectList(class UQuestingFunction* QuestingFunction, int32 FlagNumber); // 퀘스트를 받고 다시 말걸었을때 발생하는 이벤트들을 위한것. ex)어디로 이동할래? 라고 물어보는 등.

	void SetSelectDial(int32 Index);
	void SetNameText(FText Name);
	void SetDialText(FText Dial);
	
	void SelectBoxHidden();

	void Clear();

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
};
