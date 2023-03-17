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
	if (bPlayer) // ���� ����
	{		
		BattleWidget->BattleTurnInit(); // ��ų Init
		BattleWidget->SetUnitList(); // PlayPriority�� �ƴ� �ܼ��� ���� ����Ʈ
	}
	else
		BattleWidget->SetUnitList();
	
}


/*---------------------------------------------
	PlayerController->BattlWidget�� �������̽�
-----------------------------------------------*/


// ���� �����ܰ� BattleWidget�� Visible ó��
void UJRPGTemaUI::SetVisibleBattleWidget(bool bFlag)
{
	BattleWidget->SetVisible(bFlag);
}


// ���� �����϶��� BattleWidget�� ��������, ��ų�°� �� ����â�� ���̸� �ȵȴ�.
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



// �� �Լ��� �ƿ� ��� ������ ���� Ű�� ���̴�.
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
