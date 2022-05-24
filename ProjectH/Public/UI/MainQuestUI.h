// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "NPCQuest.h"
#include "MainQuestUI.generated.h"

/**
 * 
 */



UCLASS()
class PROJECTH_API UMainQuestUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	bool OpenUI(bool IsOpen);
	void OpenInfoUI(FNPCQuest NPCQuest, int32 QuestIndex);
	void QuestInfoAnimation(bool IsOpened);


	void SettingKey();
	void SetMouseOff();

	bool OtherUIOpen(); // 다른 UI가 켜져있는지 판단.

	/*---Setting UI---*/
	void OpenESCMenu(); // 만약 다른 UI가 켜져있으면 그 UI부터 지운다. ESC 이므로.


public:
	/*-----------------
		Public Value
	-----------------*/
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UQuestList* QuestList;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UQuestInfo* QuestInfo;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UESCMenu* ESCMenu;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AProjectH_PC* OwnerController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AProjectHCharacter* OwnerCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UQuestComponent* QuestComponent;


	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Fade;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* QuestInfoFade;

};
