// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGMainWidget.h"
#include "Tema/JRPG/MainUI/JRPGESCMenu.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/MainUI/JRPGMainPartyUI.h"
#include "Components/Button.h"
#include "Tema/JRPG/CustomWidget.h"


void UJRPGMainWidget::Init()
{
	ESCMenu->OwnerController = OwnerController;
	ESCMenu->OwnerMainUI = this;
	ESCMenu->Init();
	ESCMenu->Resome->OnClicked.AddDynamic(this, &UJRPGMainWidget::ReverseESC);

	MainPartyUI->OwnerController = OwnerController;
	MainPartyUI->Init();
}


void UJRPGMainWidget::PlayESCAnim(bool bPlay)
{
	PlayAnimation(ESCAnim, 0.f, 1, bPlay ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
	ESCMenu->SetVisibility(bPlay ? ESlateVisibility::Visible : ESlateVisibility::HitTestInvisible);
	if (bPlay)
	{
		if(!OwnerController->bShowMouseCursor)
			OwnerController->MouseOnOff();

		MainPartyUI->SetVisibility(ESlateVisibility::Hidden);
		OwnerController->GameType = EGameModeType::UI;
	}
	else
	{
		MainPartyUI->SetVisibility(ESlateVisibility::Visible);
		SetMouseOff();
		OwnerController->GameType = EGameModeType::Normal;
	}

}

void UJRPGMainWidget::PlayESC()
{
	if (bESCFlag)
	{
		bESCFlag = false;
		PlayESCAnim(false);
		_DEBUG("false");
	}
	else
	{
		bESCFlag = true;
		PlayESCAnim(true);
		_DEBUG("true");
	}

}


//void UJRPGMainWidget::PlayESC()
//{
//	PlayESCAnim(true);
//}


void UJRPGMainWidget::ReverseESC()
{
	PlayESCAnim(false);
}

float UJRPGMainWidget::GetESCRenderOpacity()
{
	return ESCMenu->GetRenderOpacity();
}

void UJRPGMainWidget::SetMouseOff()
{
	MouseOff();
}

void UJRPGMainWidget::MouseOff()
{
	if (OwnerController)
	{
		OwnerController->SetShowMouseCursor(false);
		OwnerController->SetInputMode(FInputModeGameOnly());
	}
}


void UJRPGMainWidget::SetPartyChange()
{
	if(MainPartyUI)
		MainPartyUI->Init();
}