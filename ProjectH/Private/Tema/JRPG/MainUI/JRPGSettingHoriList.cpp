// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGSettingHoriList.h"
#include "Tema/JRPG/MainUI/JRPGSettingPartyIcon.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Components/HorizontalBox.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"




void UJRPGSettingHoriList::Init()
{
	for (int32 i = 0; i < CharNums.Num(); i++)
	{
		UJRPGSettingPartyIcon* Icon = CreateWidget<UJRPGSettingPartyIcon>(GetWorld(), BP_UnitIcon);
		if (Icon && OwnerController)
		{	
			Icon->OwnerField = OwnerField;

			if (CharNums.IsValidIndex(i))
			{
				Icon->CharNum = CharNums[i];
				Icon->Init(OwnerController->GetPartySettingIcon(CharNums[i]));
				int cnt = OwnerController->CurrentParty.Num();
				for (int32 y = 0; y < cnt; ++y)
				{
					if (OwnerController->CurrentParty[y] == CharNums[i])
					{
						Icon->SetSelectColor(true);
						OwnerField->CurrentPartyIcons[y] = Icon;
						break;
					}				
				}	
			}
			else
			{
				Icon->CharNum = -1;
				Icon->Init(Icon->NormalTex);
			}
			// HoriList¿¡ »ý¼º.
			HoriList->AddChild(Icon);
		}
	}
}