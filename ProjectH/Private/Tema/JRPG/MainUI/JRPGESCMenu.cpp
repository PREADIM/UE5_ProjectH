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


// 파티설정 UI키기
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

// 테마 나가기.
void UJRPGESCMenu::QuitTema()
{
	if (OwnerMainUI)
	{
		OwnerMainUI->ReverseESC();
		// 여기서 테마를 정말로 나가겠냐고 한번 묻는 창 띄우고, 취소 , 확인에 따라 결정.
		// 창은 TSubclassOf 형태로 가지고있고 AddToViewPort로 띄우자.
	}
}