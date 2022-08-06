// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGTemaUI.h"
#include "Tema/JRPG/MainUI/JRPGMainWidget.h"
#include "Tema/JRPG/BattleUI/JRPGBattleWidget.h"
#include "Tema/JRPG/CustomWidget.h"

void UJRPGTemaUI::Init()
{
	CreateMainWidget();
	CreateBattleWidget();
	MainWidget->AddToViewport();
}


void UJRPGTemaUI::OpenESCMenu()
{
	if (MainWidget->IsInViewport())
	{
		MainWidget->PlayESC();
		_DEBUG("Play");
	}
}

void UJRPGTemaUI::CloseESCMenu()
{
	if (MainWidget->GetESCRenderOpacity() >= 0.1f)
	{
		MainWidget->ReverseESC();
		_DEBUG("Reverse");
	}
}


void UJRPGTemaUI::OpenBattleESCMenu()
{
	if (BattleWidget->IsInViewport())
	{
		/*if (BattleWidget->GetESCRenderOpacity() >= 0.1f)
		{
			BattleWidget->ReverseESC();
		}
		else
		{
			BattleWidget->PlayESC();
		}*/
	}
}


void UJRPGTemaUI::CreateMainWidget()
{
	if (BP_MainWidget)
	{
		MainWidget = CreateWidget<UJRPGMainWidget>(GetWorld(), BP_MainWidget);
		MainWidget->OwnerController = OwnerController;
		MainWidget->GM = GM;
		MainWidget->Init();
	}
}

void UJRPGTemaUI::CreateBattleWidget()
{
	if (BP_BattleWidget)
	{
		BattleWidget = CreateWidget<UJRPGBattleWidget>(GetWorld(), BP_BattleWidget);
		BattleWidget->OwnerController = OwnerController;
		BattleWidget->GM = GM;
		BattleWidget->Init();
	}

}



void UJRPGTemaUI::StartBattleWidget()
{
	if (MainWidget->IsInViewport())
	{
		MainWidget->RemoveFromParent();
	}

	BattleWidget->AddToViewport();
}


void UJRPGTemaUI::StartMainWidget()
{
	if (BattleWidget->IsInViewport())
	{
		BattleWidget->RemoveFromParent();
	}

	MainWidget->AddToViewport();
}



void UJRPGTemaUI::SetPartyChange()
{
	if (MainWidget && BattleWidget)
	{
		MainWidget->SetPartyChange();
		BattleWidget->SetPartyChange();
	}
}