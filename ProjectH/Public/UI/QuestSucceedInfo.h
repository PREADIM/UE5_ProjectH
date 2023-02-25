// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "NPCQuest.h"
#include "QuestSucceedInfo.generated.h"

/**
 *
 */
UCLASS(Meta = (DisableNativeTick))
class PROJECTH_API UQuestSucceedInfo : public UUserWidget
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
		class UButton* Succeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Decline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AProjectH_PC* OwnerController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UQuestComponent* QuestComponent;
	UPROPERTY(VisibleAnywhere)
		class UProjectHGameInstance* GI;


	/*---------------------------------------
		메인 퀘스트인지 아닌지 판단하여 색깔 수정
	----------------------------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor MainColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor NormalColor;

	UPROPERTY()
		FNPCQuest NPCQuest;

	int32 QuestIndex;

public:
	void Init();
	void SetText();
	void SetColorPenSize();

	void BindingFunction();

	UFUNCTION()
		void SucceedClick();
	UFUNCTION()
		void DeclineClick();
	UFUNCTION(BlueprintImplementableEvent)
		void SetOutline(bool bMain);

};
