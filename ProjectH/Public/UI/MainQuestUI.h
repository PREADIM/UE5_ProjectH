// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "NPCQuest.h"
#include "MainQuestUI.generated.h"

/**
 * 
 */



UCLASS(Meta = (DisableNativeTick))
class PROJECTH_API UMainQuestUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	bool OpenUI(bool IsOpen);
	void OpenInfoUI(FNPCQuest NPCQuest, int32 QuestIndex);
	void OpenSucceedInfo(FNPCQuest NPCQuest, int32 QuestIndex);
	void QuestInfoAnimation(bool IsOpened);
	void QuestSucceedInfoAnimation(bool IsOpened);


	void SettingKey();
	void SetMouseOff();
	void MouseOff();

	bool OtherUIOpen(); // 다른 UI가 켜져있는지 판단.

	/*---Setting UI---*/
	void OpenESCMenu(); // 만약 다른 UI가 켜져있으면 그 UI부터 지운다. ESC 이므로.

	void OpenDialogue();
	UFUNCTION()
		void CloseDialogue();
	bool bDialogueOpen();

	void OpenInteract();
	void CloseInteract();

	void SetName(FString Name);

	void SetMouseCursorCenter();

	void SetDialogueNPC(class AQuestNPCBase* NPC);

public:
	/*-----------------
		Public Value
	-----------------*/
	UPROPERTY(meta = (BindWidget))
		class UQuestList* QuestList;
	UPROPERTY(meta = (BindWidget))
		class UQuestInfo* QuestInfo;
	UPROPERTY(meta = (BindWidget))
		class UQuestSucceedInfo* QuestSucceedInfo;
	UPROPERTY(meta = (BindWidget))
		class UESCMenu* ESCMenu;
	UPROPERTY(meta = (BindWidget))
		class UDialogueWidget* Dialogue;
	UPROPERTY(meta = (BindWidget))
		class UInteractWidget* InteractWidget;


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
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* QuestSucceedInfoFade;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* DialFade;

};
