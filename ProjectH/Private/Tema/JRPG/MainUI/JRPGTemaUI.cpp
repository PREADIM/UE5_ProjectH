// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGTemaUI.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/MainUI/JRPGMainWidget.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/BattleUI/JRPGBattleWidget.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/BattleUI/JRPGBattleWidget.h"
#include "Tema/JRPG/BattleUI/LockOnWidget.h"

void UJRPGTemaUI::Init()
{
	CreateMainWidget();
	CreateBattleWidget();
	MainWidget->AddToViewport();
}


void UJRPGTemaUI::OpenESCMenu()
{
	if (MainWidget->IsInViewport())
		MainWidget->PlayESC();
}

void UJRPGTemaUI::CloseESCMenu()
{
	if (MainWidget->GetESCRenderOpacity() >= 0.1f)
		MainWidget->ReverseESC();
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
		OwnerController->LastWidget.Empty();
	}

	BattleWidget->AddToViewport();
	BattleWidget->VisibleLockOn();
}


void UJRPGTemaUI::StartMainWidget()
{
	if (BattleWidget->IsInViewport())
	{
		BattleWidget->HiddenLockOn();
		BattleWidget->RemoveFromParent();
	}

	OwnerController->SetInputMode(FInputModeGameOnly());
	OwnerController->SetShowMouseCursor(false);
	MainWidget->AddToViewport();
}



void UJRPGTemaUI::SetPartyChange()
{
	if (MainWidget && BattleWidget)
		MainWidget->SetPartyChange();
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
		BattleWidget->BattleTurnInit();
}



void UJRPGTemaUI::BattleTurnStart(bool bPlayer)
{
	if (bPlayer) // 오너 유닛
	{		
		BattleWidget->BattleTurnInit(); // 스킬 Init
		BattleWidget->SetUnitList(); // PlayPriority가 아닌 단순한 유닛 리스트
	}
	else
		BattleWidget->SetUnitList();
	
}


/*---------------------------------------------
	PlayerController->BattlWidget의 인터페이스
-----------------------------------------------*/


// 락온 아이콘과 BattleWidget의 Visible 처리
void UJRPGTemaUI::SetVisibleBattleWidget(bool bFlag)
{
	BattleWidget->SetVisible(bFlag);
}


// 적의 차례일때는 BattleWidget은 보이지만, 스킬셋과 적 선택창은 보이면 안된다.
void UJRPGTemaUI::SkillAndListButtonHidden(bool bFlag)
{
	BattleWidget->SkillAndListButtonHidden(bFlag);
}

void UJRPGTemaUI::EnermyListSetup()
{
	BattleWidget->EnermyListInit();
}

void UJRPGTemaUI::HiddenLockOn()
{
	BattleWidget->HiddenLockOn();
}



// 이 함수는 아예 모든 위젯을 껏다 키는 것이다.
void UJRPGTemaUI::BattleUIOnOff(bool bOnOff)
{
	if (bOnOff)
	{
		BattleWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		BattleWidget->LockOnIcon->SetRenderOpacity(1.0f);	
	}
	else
	{
		BattleWidget->SetVisibility(ESlateVisibility::Hidden);
		BattleWidget->LockOnIcon->SetRenderOpacity(0.0f);		
	}
}


void UJRPGTemaUI::PlayPriority()
{
	BattleWidget->PlayPriority();
}
