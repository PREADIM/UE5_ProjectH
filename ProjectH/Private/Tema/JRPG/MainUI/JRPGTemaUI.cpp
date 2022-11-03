// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGTemaUI.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/MainUI/JRPGMainWidget.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/BattleUI/JRPGBattleWidget.h"
#include "Tema/JRPG/JRPGUnit.h"

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
		BattleWidget->HiddenLockOn();
		BattleWidget->RemoveFromParent();
	}

	OwnerController->SetInputMode(FInputModeGameOnly());
	OwnerController->GameType = EGameModeType::Normal;
	OwnerController->SetShowMouseCursor(false);
	MainWidget->AddToViewport();
}



void UJRPGTemaUI::SetPartyChange()
{
	if (MainWidget && BattleWidget)
	{
		MainWidget->SetPartyChange();
	}
}


bool UJRPGTemaUI::MainIsInViewport()
{
	return MainWidget->IsInViewport();
}

bool UJRPGTemaUI::BattleIsInViewport()
{
	return BattleWidget->IsInViewport();
}


void UJRPGTemaUI::PlayBattleWidget()
{
	if (BattleWidget)
	{
		if (GM->SetUnitList[0].Unit->PlayerType == EPlayerType::Player)
			BattleWidget->BattleTurnInit();
		else
		{
			BattleWidget->EnermyTurnFirst();
		}
		BattleWidget->PlayPriority(); // 우선순위로 짜여진 캐릭터 리스트 위젯 애님 실행.	
	}
}



void UJRPGTemaUI::BattleTurnStart(bool bFlag)
{
	if (bFlag) // 오너 유닛
	{
		BattleWidget->BattleTurnInit(); // 스킬 Init
		BattleWidget->SetUnitList(); // PlayPriority가 아닌 단순한 유닛 리스트
	}
	else
	{
		BattleWidget->SetUnitList();
	}
	
}

void UJRPGTemaUI::SetVisibleBattleWidget(bool bFlag)
{
	BattleWidget->SetVisible(bFlag);
}

void UJRPGTemaUI::EnermyTurnWidget(bool bFlag)
{
	BattleWidget->EnermyTurnHidden(bFlag);
}

void UJRPGTemaUI::EnermyListSetup()
{
	BattleWidget->EnermyListBeginInit();
}


void UJRPGTemaUI::TargetToRotation()
{
	BattleWidget->TargetToRotation();
}


void UJRPGTemaUI::EnermyTargetToRotation()
{
	BattleWidget->EnermyTargetToRotation();
}