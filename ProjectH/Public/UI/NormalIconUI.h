// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "NormalIconUI.generated.h"

/**
 * 
 */

UENUM()
enum class EQuestState : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	SubQuest UMETA(DisplayName = "SubQuest"),
	MainQuest UMETA(DisplayName = "MainQuest"),
	SubQuesting UMETA(DisplayName = "SubQuesting"),
	MainQuesting UMETA(DisplayName = "MainQuesting"),
	SubSucceedQuest UMETA(DisplayName = "SubSucceedQuest"),
	MainSucceedQuest UMETA(DisplayName = "MainSucceedQuest")
};


UCLASS(Meta = (DisableNativeTick))
class PROJECTH_API UNormalIconUI : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UUserWidget* SubQuestWidget;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UUserWidget* MainQuestWidget;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UUserWidget* SubQuestingWidget;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UUserWidget* MainQuestingWidget;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UUserWidget* SubSucceedWidget;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UUserWidget* MainSucceedWidget;

public:
	virtual void NativeConstruct();
	void SetRenderIcon(EQuestState State);

};
