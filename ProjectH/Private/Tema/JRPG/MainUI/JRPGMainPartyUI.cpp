// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGMainPartyUI.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Components/VerticalBox.h"
#include "Tema/JRPG/MainUI/JRPGPartySlot.h"


void UJRPGMainPartyUI::Init()
{
	if (OwnerController)
	{
		GM = OwnerController->GM;
		CurrentParty = OwnerController->CurrentParty;
		SetCurrentPartyList(); // 현재 파티리스트를 UI로 띄우기위한 작업.
	}
}


void UJRPGMainPartyUI::SetCurrentPartyList()
{
	// JRPGPartySlot을 VerticalBox에 AddChild 하는 방식.
	if (!BP_PartySlot)
		return;

	if (OwnerController)
	{
		MainPartyList->ClearChildren();

		for (int32 i = 0; i < CurrentParty.Num(); i++)
		{
			UJRPGPartySlot* PartySlot = CreateWidget<UJRPGPartySlot>(GetWorld(), BP_PartySlot);

			if (PartySlot)
			{
				// 컨트롤러에서 데이터 베이스에서 검색해서 가져온 값으로 Init 전해주기
				// ★ 컨트롤러에 FJRPGUnitUIStruct형 데이터 베이스를 검색할수있게 데이터 테이블 하나 가지고있기.
				// 그리고 해당 데이터에서 CurrentParty의 넘버를 검색해서 정보 가져와서 Init에 넣기

				FJRPGUnitUIStruct* UnitUI = OwnerController->GetUnitUI(CurrentParty[i]);
				if (UnitUI)
				{
					PartySlot->Init(UnitUI->CharTex, UnitUI->CharName, i + 1);
					PartySlot->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));
					MainPartyList->AddChild(PartySlot);
					PartySlots.Add(PartySlot);
				}
			
			}
		}
	}
}