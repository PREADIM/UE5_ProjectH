// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainQuestUI.h"
#include "Character/ProjectHCharacter.h"
#include "Controller/ProjectH_PC.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "UI/QuestList.h"
#include "UI/QuestInfo.h"
#include "UI/ESCMenu.h"
#include "UI/OptionMenu.h"
#include "Components/CanvasPanel.h"


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
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &UMainQuestUI::SetMouseOff, Fade->GetEndTime(), false);
	}
}

void UMainQuestUI::SetMouseOff()
{
	if (!OtherUIOpen())
	{
		OwnerController->SetShowMouseCursor(false);
		OwnerController->SetInputMode(FInputModeGameOnly());
	}

}


bool UMainQuestUI::OtherUIOpen()
{
	if (QuestList->GetRenderOpacity() == 1.0f)
	{
		_DEBUG("QuestList Open true");
		return true;
	}
	if (ESCMenu->SettingCanvas->GetRenderOpacity() == 1.0f)
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
			OwnerController->SetShowMouseCursor(true);
			OwnerController->SetInputMode(FInputModeGameAndUI());
		}
	}

}