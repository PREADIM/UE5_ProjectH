// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGPartySettingUI.h"
#include "Tema/JRPG/MainUI/JRPGMainWidget.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Components/Button.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/CustomWidget.h"


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
		if (BP_PartyField)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			PartyField = GetWorld()->SpawnActor<APartySettingField>(BP_PartyField, FTransform(FVector(7441.f, -14870.f, 9812.f)), SpawnParameters);
			CurrentParty = OwnerController->CurrentParty;
		}	
	}

}


void UJRPGPartySettingUI::SetCurrentParty()
{
	if (PartyField)
	{
		for (int32 i = 0; i < CurrentParty.Num(); i++)
		{
			FTransform UnitLocation;
			switch (i)
			{
			case 0:
				UnitLocation = PartyField->Unit1->GetComponentTransform();
				break;
			case 1:
				UnitLocation = PartyField->Unit2->GetComponentTransform();
				break;
			case 2:
				UnitLocation = PartyField->Unit3->GetComponentTransform();
				break;
			case 3:
				UnitLocation = PartyField->Unit4->GetComponentTransform();
				break;
			}

			SpawnChars.Add(GM->GetCharacterSpawn(CurrentParty[i], UnitLocation));
		}
	}	
}

void UJRPGPartySettingUI::ExitWidget()
{
	for (AJRPGUnit* Units : SpawnChars)
	{
		Units->Destroy();
	}

	SpawnChars.Empty();
	OwnerController->OnPossess(OwnerController->RepreCharacter);
	OwnerController->MouseOff();
	OwnerMainUI->SetVisibility(ESlateVisibility::Visible);
	RemoveFromParent();
	OwnerController->GameType = EGameModeType::Normal;
}