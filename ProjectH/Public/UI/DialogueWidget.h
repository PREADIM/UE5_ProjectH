// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "Dialogue/TextNAnim.h"
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

	UPROPERTY(VisibleAnywhere)
		class AQuestNPCBase* OwnerNPC;
	UPROPERTY(VisibleAnywhere)
		int32 OwnerQuestNum = 0;

	/*UPROPERTY()
		class APlayerController* OwnerController;*/
	UPROPERTY()
		class UMainQuestUI* OwnerMainWidget;

	UPROPERTY()
		TArray<FText> NormalDialogue;
	int32 DialIndex = 0;
	
	UPROPERTY()
		TArray<FTextNAnim> SelectText;
	int32 SelectTextIndex = 0;




public:
	void NPCDialogue();
	void SetNormalDialText(int32 index);
	void SetCanQuestList();

	void SetSelectDial(int32 Index);
	void SetNameText(FText Name);
	void SetDialText(FText Dial);
	
	void SelectBoxHidden();

	void Clear();

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
};
