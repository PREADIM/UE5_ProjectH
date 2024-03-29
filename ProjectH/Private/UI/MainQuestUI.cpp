// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainQuestUI.h"
#include "Character/ProjectHCharacter.h"
#include "Controller/ProjectH_PC.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "UI/QuestList.h"
#include "UI/QuestInfo.h"
#include "UI/QuestSucceedInfo.h"
#include "UI/ESCMenu.h"
#include "UI/OptionMenu.h"
#include "UI/HelpMenu.h"
#include "UI/DialogueWidget.h"
#include "UI/InteractWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Kismet/KismetmathLibrary.h"
#include "GameMode/ProjectHGameInstance.h"


void UMainQuestUI::Init()
{
	QuestComponent = OwnerCharacter->GetQuestComponent();

	if (QuestComponent && OwnerController)
	{
		QuestList->QuestComponent = QuestComponent;
		QuestList->Init();

		QuestInfo->OwnerController = OwnerController;
		QuestInfo->QuestComponent = QuestComponent;
		QuestInfo->GI = Cast<class UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
		QuestInfo->BindingFunction(); // 수락, 거절 버튼 바인딩.

		QuestSucceedInfo->OwnerController = OwnerController;
		QuestSucceedInfo->QuestComponent = QuestComponent;
		QuestSucceedInfo->GI = Cast<class UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
		QuestSucceedInfo->BindingFunction(); // 수락, 거절 버튼 바인딩.

		ESCMenu->OwnerController = OwnerController;
		ESCMenu->Init();

		//DialWidget->OwnerController = OwnerController;
		Dialogue->OwnerMainWidget = this;
		Dialogue->Exit->OnClicked.AddDynamic(this, &UMainQuestUI::CloseDialogue);
		Dialogue->OwnerController = OwnerController;
	}

}


bool UMainQuestUI::OpenUI(bool IsOpen)
{
	//IsOpen이 true면 켜져있다는 뜻, false면 꺼져있으니 이제 킨다는 뜻.
	PlayAnimation(Fade, 0.f, 1, IsOpen ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
	if (QuestList)
	{
		QuestList->SetVisibility(IsOpen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
		if (!IsOpen)
		{
			SetMouseCursorCenter();
			OwnerController->SetShowMouseCursor(true);
			OwnerController->SetInputMode(FInputModeGameAndUI());
		}
		else
		{
			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &UMainQuestUI::SetMouseOff, Fade->GetEndTime(), false);
		}
	}

	return !IsOpen; // 토글
}


void UMainQuestUI::OpenInfoUI(FNPCQuest NPCQuest, int32 QuestIndex)
{
	if (QuestInfo->GetRenderOpacity() > 0.2f)
		return;
	else
	{
		QuestInfo->QuestIndex = QuestIndex;
		QuestInfo->NPCQuest = NPCQuest;

		QuestInfo->Init();
		QuestInfoAnimation(false);
	}
}

void UMainQuestUI::OpenSucceedInfo(FNPCQuest NPCQuest, int32 QuestIndex)
{
	if (QuestSucceedInfo->GetRenderOpacity() > 0.2f)
	{
		return;
	}
	else
	{
		QuestSucceedInfo->QuestIndex = QuestIndex;
		QuestSucceedInfo->NPCQuest = NPCQuest;

		QuestSucceedInfo->Init();
		QuestSucceedInfoAnimation(false);
	}

}


//true면 이제 끄는 것, false가 오면 이제 키는 것.
void UMainQuestUI::QuestInfoAnimation(bool IsOpened)
{
	PlayAnimation(QuestInfoFade, 0.f, 1, IsOpened ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
	QuestInfo->SetVisibility(IsOpened ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
	// true면 무언가 켜져있다는 뜻, false면 켜져있는데 없다는 뜻.
	if (!IsOpened)
	{
		OwnerController->SetShowMouseCursor(true);
		OwnerController->SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		SetMouseOff();
	}
}

//true면 이제 끄는 것, false가 오면 이제 키는 것.
void UMainQuestUI::QuestSucceedInfoAnimation(bool IsOpened)
{
	PlayAnimation(QuestSucceedInfoFade, 0.f, 1, IsOpened ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
	QuestSucceedInfo->SetVisibility(IsOpened ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
	// true면 무언가 켜져있다는 뜻, false면 켜져있는데 없다는 뜻.
	if (!IsOpened)
	{
		OwnerController->SetShowMouseCursor(true);
		OwnerController->SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		SetMouseOff();
	}
}


// 키보드 단축키로 옵션키를 눌렀다.
void UMainQuestUI::SettingKey()
{
	if (ESCMenu->SettingKeyClick())
	{
		SetMouseCursorCenter();
		OwnerController->SetShowMouseCursor(true);
		OwnerController->SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &UMainQuestUI::SetMouseOff, ESCMenu->OptionMenu->OptionFade->GetEndTime() + 0.1f, false);
	}

}

void UMainQuestUI::SetMouseOff()
{
	if (!OtherUIOpen())
	{
		MouseOff();
	}
}

void UMainQuestUI::MouseOff()
{
	if (OwnerController)
	{
		OwnerController->SetShowMouseCursor(false);
		OwnerController->SetInputMode(FInputModeGameOnly());
	}
}



bool UMainQuestUI::OtherUIOpen()
{
	if (Dialogue->GetRenderOpacity() > 0.2f)
		return true;

	if (QuestList->GetRenderOpacity() > 0.2f)
		return true;

	if (ESCMenu->HelpMenu->IsInViewport())
		return true;

	if (ESCMenu->SettingCanvas->GetRenderOpacity() > 0.2f)
		return true;

	if (ESCMenu->OptionMenu->IsInViewport())
		return true;


	return false;
}



void UMainQuestUI::OpenESCMenu()
{
	/* 차례대로 ESC 눌렀을때 열려있는 창 닫기. */
	if (QuestInfo->GetRenderOpacity() == 1.0f)
	{
		QuestInfoAnimation(true);
		return;
	}

	if (QuestList->GetRenderOpacity() == 1.0f)
	{
		OpenUI(true);
		OwnerController->IsOpen = false;
		return;
	}

	if (ESCMenu->HelpMenu->IsInViewport())
	{
		ESCMenu->HelpMenu->HelpUIAnim(true);
		MouseOff();
		return;
	}

	if (Dialogue->GetRenderOpacity() == 1.0f)
	{
		CloseDialogue();
		return;
	}

	// ESC 메뉴를 켜야할때 이미 켜져있는지 아닌지 판단.
	if (ESCMenu->SettingCanvas->GetRenderOpacity() == 1.0f) //ESC메뉴 끄기
	{
		ESCMenu->ESCMenuAnimation(true); // 셋팅창 애니메이션.
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &UMainQuestUI::SetMouseOff, ESCMenu->ESCMenuFade->GetEndTime(), false);
		return;
	}
	else if(ESCMenu->SettingCanvas->GetRenderOpacity() == 0.0f) // ESC메뉴 키기
	{
		// 옵션창이 켜져있다면 자연스럽게 ESC 메뉴로 넘어가야 하므로.
		if (ESCMenu->OptionMenu->IsInViewport())
		{
			ESCMenu->OptionMenu->OptionAnimation(true);
			ESCMenu->ESCMenuAnimation(false); // 셋팅창만 애니메이션 켜기
			return;
		}
		else
		{
			ESCMenu->ESCMenuAnimation(false);
			SetMouseCursorCenter();
			OwnerController->SetShowMouseCursor(true);
			OwnerController->SetInputMode(FInputModeGameAndUI());
		}
	}

}


void UMainQuestUI::OpenDialogue()
{
	PlayAnimation(DialFade, 0.f, 1, EUMGSequencePlayMode::Forward);
	Dialogue->SetVisibility(ESlateVisibility::Visible);

	SetMouseCursorCenter();
	OwnerController->SetShowMouseCursor(true);
	OwnerController->SetInputMode(FInputModeGameAndUI());
}

void UMainQuestUI::CloseDialogue()
{
	Dialogue->SetRenderOpacity(0.0f);
	Dialogue->SetVisibility(ESlateVisibility::Hidden);
	Dialogue->Clear();

	OwnerController->SetShowMouseCursor(false);
	OwnerController->SetInputMode(FInputModeGameOnly());

	OwnerCharacter->InteractCollisionSetUp();
}


bool UMainQuestUI::bDialogueOpen()
{
	if (Dialogue->GetRenderOpacity() > 0.0f)
		return true;
	else if (QuestInfo->GetRenderOpacity() > 0.0f)
		return true;
	else
		return false;
}

void UMainQuestUI::OpenInteract()
{
	InteractWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	InteractWidget->PlayAnim();
}


void UMainQuestUI::CloseInteract()
{
	InteractWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UMainQuestUI::SetName(FString Name)
{
	InteractWidget->SetName(Name);
}

/* 마우스 커서 센터로 나오게 하기. */
void UMainQuestUI::SetMouseCursorCenter()
{
	int32 x, y;
	OwnerController->GetViewportSize(x, y);
	OwnerController->SetMouseLocation(UKismetMathLibrary::FTrunc(x / 2), UKismetMathLibrary::FTrunc(y / 2));
}

void UMainQuestUI::SetDialogueNPC(AQuestNPCBase* NPC)
{
	Dialogue->OwnerNPC = NPC;
	Dialogue->NPCDialogue();
}
