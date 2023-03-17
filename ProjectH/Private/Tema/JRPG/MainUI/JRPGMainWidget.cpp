// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGMainWidget.h"
#include "Tema/JRPG/MainUI/JRPGESCMenu.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/MainUI/JRPGMainPartyUI.h"
#include "Tema/JRPG/CustomWidget.h"


void UJRPGMainWidget::Init()
{
	ESCMenu->OwnerController = OwnerController;
	ESCMenu->OwnerMainUI = this;
	ESCMenu->Init();

	MainPartyUI->OwnerController = OwnerController;
	MainPartyUI->Init();
}


void UJRPGMainWidget::PlayESCAnim(bool bPlay)
{
	PlayAnimation(ESCAnim, 0.f, 1, bPlay ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
	ESCMenu->SetVisibility(bPlay ? ESlateVisibility::Visible : ESlateVisibility::HitTestInvisible);
	if (bPlay)
	{
		MainPartyUI->SetVisibility(ESlateVisibility::Hidden); /* 평상시 메인메뉴 아이콘 가리기 */

		if (!OwnerController->bShowMouseCursor)
			OwnerController->MouseOn();
	}
	else
	{
		MainPartyUI->SetVisibility(ESlateVisibility::Visible);
		OwnerController->MouseOff();
	}

}

void UJRPGMainWidget::PlayESC()
{
	if (ESCMenu->GetRenderOpacity() <= 0.1f)
		PlayESCAnim(true);		
	else
		PlayESCAnim(false);
}


void UJRPGMainWidget::ReverseESC()
{
	PlayESCAnim(false);
}

float UJRPGMainWidget::GetESCRenderOpacity()
{
	return ESCMenu->GetRenderOpacity();
}


void UJRPGMainWidget::SetPartyChange()
{
	if(MainPartyUI)
		MainPartyUI->Init();
}