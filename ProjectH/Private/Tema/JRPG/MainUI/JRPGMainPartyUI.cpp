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
		SetCurrentPartyList(); // ���� ��Ƽ����Ʈ�� UI�� �������� �۾�.
	}
}


void UJRPGMainPartyUI::SetCurrentPartyList()
{
	// JRPGPartySlot�� VerticalBox�� AddChild �ϴ� ���.
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
				// ��Ʈ�ѷ����� ������ ���̽����� �˻��ؼ� ������ ������ Init �����ֱ�
				// �� ��Ʈ�ѷ��� FJRPGUnitUIStruct�� ������ ���̽��� �˻��Ҽ��ְ� ������ ���̺� �ϳ� �������ֱ�.
				// �׸��� �ش� �����Ϳ��� CurrentParty�� �ѹ��� �˻��ؼ� ���� �����ͼ� Init�� �ֱ�

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