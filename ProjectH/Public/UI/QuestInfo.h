// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "NPCQuest.h"
#include "QuestInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UQuestInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* Canvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ColorPen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* QuestNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* QuestDescriptionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Accept;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Decline;


	UPROPERTY(VisibleAnywhere)
		FString QuestName;
	UPROPERTY(VisibleAnywhere)
		FString QuestDescription;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AProjectH_PC* OwnerController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UQuestComponent* QuestComponent;



	/*---------------------------------------
		메인 퀘스트인지 아닌지 판단하여 색깔 수정
	----------------------------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor MainColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor NormalColor;



	FNPCQuest NPCQuest;
	int32 QuestIndex;

public:
	void Init();
	void SetText();
	void SetColorPenSize();

	void BindingFunction();

	UFUNCTION()
		void AcceptClick();
	UFUNCTION()
		void DeclineClick();
	UFUNCTION(BlueprintImplementableEvent)
		void SetOutline(bool bMain);
	
};
