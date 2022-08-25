// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGPartySettingUI.h"
#include "Tema/JRPG/MainUI/JRPGSettingPartySlot.h"
#include "Tema/JRPG/MainUI/JRPGMainWidget.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Components/Button.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"
#include "Tema/JRPG/JRPGUnit.h"


void UJRPGPartySettingUI::SetCloseFunction()
{
	ExitWidget();
}


void UJRPGPartySettingUI::Init()
{
	if (OwnerController)
	{
		GM = OwnerController->GM;
		Exit->OnClicked.AddDynamic(this, &UJRPGPartySettingUI::ExitWidget);
	}

}


void UJRPGPartySettingUI::ExitWidget()
{
	if (PartyField)
	{
		for (int32 i = 0; i < 3; i++)
		{
			if (PartyField->SpawnChars[i] != nullptr)
			{
				PartyField->SpawnChars[i]->Destroy();
				PartyField->SpawnChars[i] = nullptr;
			}

		}

		PartyField->SpawnChars.Empty();
		PartyField->SettingUI->OwnerField = nullptr;
		PartyField->Destroy();
		OwnerController->SetRepreCharacterSpawnUI(OwnerController->CurrentPartyIndex);
		OwnerController->MouseOff();
		OwnerMainUI->SetVisibility(ESlateVisibility::Visible);

		// ★★ 이 두개는 꼭 해야한다.
		RemoveFromParent();
		OwnerController->LastWidget.Pop();

		OwnerController->GameType = EGameModeType::Normal;
		UE_LOG(LogTemp, Warning, TEXT("ExitWidget"));
	}
}