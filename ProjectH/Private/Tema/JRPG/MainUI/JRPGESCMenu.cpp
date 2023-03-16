// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGESCMenu.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/MainUI/JRPGPartySettingUI.h"
#include "Tema/JRPG/MainUI/JRPGMainWidget.h"
#include "Components/Button.h"
#include "Tema/JRPG/CustomWidget.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"

void UJRPGESCMenu::Init()
{
	PartySetting->OnClicked.AddDynamic(this, &UJRPGESCMenu::PartySet);
	Quit->OnClicked.AddDynamic(this, &UJRPGESCMenu::QuitTema);
}


// 파티설정 UI키기
void UJRPGESCMenu::PartySet()
{
	if (OwnerController)
	{
		if (BP_PartyField)
		{
			if (OwnerMainUI)
			{
				SpawnPartyField();
				if (PartyField)
				{
					PartyField->SetCurrentParty();
					//OwnerMainUI->ReverseESC();
					OwnerController->OnPossess(Cast<APawn>(PartyField));
					OwnerController->MouseOn();
					OwnerController->GameType = EGameModeType::UI;
				}
				OwnerMainUI->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UJRPGESCMenu::SpawnPartyField()
{
	if (BP_PartyField)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		PartyField = GetWorld()->SpawnActor<APartySettingField>(BP_PartyField, FTransform(FVector(7441.f, -14870.f, 9812.f)), SpawnParameters);
		PartyField->OwnerController = OwnerController;
		PartyField->Init(OwnerMainUI);
		UE_LOG(LogTemp, Warning, TEXT("SpawnPartyField"));
	}
}

// 테마 나가기.
void UJRPGESCMenu::QuitTema()
{
	if (OwnerMainUI && BP_QuitWidget)
	{	
		QuitWidget = CreateWidget<UUserWidget>(GetWorld(), BP_QuitWidget);
		if (QuitWidget)
		{
			OwnerMainUI->ReverseESC();
			OwnerController->MouseOn();
			QuitWidget->AddToViewport();
		}
	}
}

