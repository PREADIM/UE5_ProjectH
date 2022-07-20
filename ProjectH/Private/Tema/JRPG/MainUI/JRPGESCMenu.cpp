// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGESCMenu.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/MainUI/JRPGPartySettingUI.h"
#include "Tema/JRPG/MainUI/JRPGMainWidget.h"
#include "Components/Button.h"
#include "Tema/JRPG/CustomWidget.h"

void UJRPGESCMenu::Init()
{
	PartySetting->OnClicked.AddDynamic(this, &UJRPGESCMenu::PartySet);
	Quit->OnClicked.AddDynamic(this, &UJRPGESCMenu::QuitTema);

	if (BP_PartySettingUI)
	{
		PartySettingUI = CreateWidget<UJRPGPartySettingUI>(GetWorld(), BP_PartySettingUI);
		PartySettingUI->OwnerController = OwnerController;
		PartySettingUI->OwnerMainUI = OwnerMainUI;
		PartySettingUI->Init();
	}
}


// ��Ƽ���� UIŰ��
void UJRPGESCMenu::PartySet()
{
	if (OwnerController)
	{
		if (PartySettingUI)
		{
			if (OwnerMainUI)
			{
				PartySettingUI->SetCurrentParty();
				OwnerMainUI->ReverseESC();
				OwnerController->OnPossess(Cast<APawn>(PartySettingUI->PartyField));
				OwnerController->MouseOn();
				OwnerController->LastWidget.Add(PartySettingUI);
				OwnerMainUI->SetVisibility(ESlateVisibility::Hidden);
				PartySettingUI->AddToViewport();
			}
		}
	}
}

// �׸� ������.
void UJRPGESCMenu::QuitTema()
{
	if (OwnerMainUI)
	{
		OwnerMainUI->ReverseESC();
		// ���⼭ �׸��� ������ �����ڳİ� �ѹ� ���� â ����, ��� , Ȯ�ο� ���� ����.
		// â�� TSubclassOf ���·� �������ְ� AddToViewPort�� �����.
	}
}