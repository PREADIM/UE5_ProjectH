// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGSettingHoriList.h"
#include "Tema/JRPG/MainUI/JRPGSettingPartyIcon.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Components/HorizontalBox.h"



void UJRPGSettingHoriList::Init()
{
	for (int32 i = 0; i < 4; i++)
	{
		UJRPGSettingPartyIcon* Icon = CreateWidget<UJRPGSettingPartyIcon>(GetWorld(), BP_UnitIcon);
		if (Icon && OwnerController)
		{	
			Icon->OwnerField = OwnerField;

			if (CharNums.IsValidIndex(i))
			{
				Icon->CharNum = CharNums[i];
				Icon->Init(OwnerController->GetPartySettingIcon(CharNums[i]));			
			}
			else
			{
				Icon->Init(Icon->NormalTex);
			}
			// HoriList¿¡ »ý¼º.
			HoriList->AddChild(Icon);
		}
	}
}