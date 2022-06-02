// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "HelpMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UHelpMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APlayerController* OwnerController;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* HelpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* PrevButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> BP_KeyQuide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> BP_QuestGuide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> BP_GameGoal;

public:
	void Init();

	void HelpUIAnim(bool IsOpened);
	void HelpMenuRemove();

	UFUNCTION(BlueprintCallable)
		void InputMode();

};
