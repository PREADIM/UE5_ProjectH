// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainQuestUI.h"
#include "Character/ProjectHCharacter.h"
#include "Controller/ProjectH_PC.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "UI/QuestList.h"
#include "UI/QuestInfo.h"
#include "UI/ESCMenu.h"
#include "UI/OptionMenu.h"
#include "UI/HelpMenu.h"
#include "UI/DialogueWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Kismet/KismetmathLibrary.h"


void UMainQuestUI::Init()
{
	QuestComponent = OwnerCharacter->GetQuestComponent();

	if (QuestComponent && OwnerController)
	{
		QuestList->QuestComponent = QuestComponent;
		QuestList->Init();

		QuestInfo->OwnerController = OwnerController;
		QuestInfo->QuestComponent = QuestComponent;
		QuestInfo->BindingFunction(); // ����, ���� ��ư ���ε�.

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
	//IsOpen�� true�� �����ִٴ� ��, false�� ���������� ���� Ų�ٴ� ��.
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

	return !IsOpen; // ���
}


void UMainQuestUI::OpenInfoUI(FNPCQuest NPCQuest, int32 QuestIndex)
{
	if (QuestInfo->GetRenderOpacity() > 0.2f)
	{
		return;
	}
	else
	{
		QuestInfo->QuestIndex = QuestIndex;
		QuestInfo->QuestName = NPCQuest.QuestName;
		QuestInfo->QuestDescription = NPCQuest.QuestDescription;
		QuestInfo->NPCQuest = NPCQuest;

		QuestInfo->Init();
		QuestInfoAnimation(false);
	}
}


//true�� ���� ���� ��, false�� ���� ���� Ű�� ��.
void UMainQuestUI::QuestInfoAnimation(bool IsOpened)
{
	PlayAnimation(QuestInfoFade, 0.f, 1, IsOpened ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
	QuestInfo->SetVisibility(IsOpened ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
	// true�� ���� �����ִٴ� ��, false�� �����ִµ� ���ٴ� ��.
	if (!IsOpened)
	{
		OwnerController->SetShowMouseCursor(true);
		OwnerController->SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		/*FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &UMainQuestUI::SetMouseOff, Fade->GetEndTime(), false);*/
		SetMouseOff();
	}
}


// Ű���� ����Ű�� �ɼ�Ű�� ������.
void UMainQuestUI::SettingKey()
{
	if (ESCMenu->SettingKeyClick())
	{
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
	{
		_DEBUG("Dialogue Open true");
		return true;
	}

	if (QuestList->GetRenderOpacity() > 0.2f)
	{
		_DEBUG("QuestList Open true");
		return true;
	}
	if (ESCMenu->HelpMenu->IsInViewport())
	{
		return true;
	}
	if (ESCMenu->SettingCanvas->GetRenderOpacity() > 0.2f)
	{
		_DEBUG("ESCMenu Open true");
		return true;
	}
	if (ESCMenu->OptionMenu->IsInViewport())
	{
		_DEBUG("OptionMenu Open true");
		return true;
	}


	_DEBUG("Other UI Open false");
	return false;
}



void UMainQuestUI::OpenESCMenu()
{
	/* ���ʴ�� ESC �������� �����ִ� â �ݱ�. */
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

	// ESC �޴��� �Ѿ��Ҷ� �̹� �����ִ��� �ƴ��� �Ǵ�.
	if (ESCMenu->SettingCanvas->GetRenderOpacity() == 1.0f)
	{
		ESCMenu->ESCMenuAnimation(true); // ����â �ִϸ��̼�.
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &UMainQuestUI::SetMouseOff, ESCMenu->ESCMenuFade->GetEndTime(), false);
		return;
	}
	else if(ESCMenu->SettingCanvas->GetRenderOpacity() == 0.0f)
	{
		// �ɼ�â�� �����ִٸ� �ڿ������� ESC �޴��� �Ѿ�� �ϹǷ�.
		if (ESCMenu->OptionMenu->IsInViewport())
		{
			ESCMenu->OptionMenu->OptionAnimation(true);
			ESCMenu->ESCMenuAnimation(false); // ����â�� �ִϸ��̼� �ѱ�
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
	Dialogue->SetVisibility(ESlateVisibility::HitTestInvisible);
	Dialogue->Clear();

	OwnerController->SetShowMouseCursor(false);
	OwnerController->SetInputMode(FInputModeGameOnly());
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


/* ���콺 Ŀ�� ���ͷ� ������ �ϱ�. */
void UMainQuestUI::SetMouseCursorCenter()
{
	int32 x, y;
	OwnerController->GetViewportSize(x, y);
	OwnerController->SetMouseLocation(UKismetMathLibrary::FTrunc(x / 2), UKismetMathLibrary::FTrunc(y / 2));
}