// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGSettingPartyIcon.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"
#include "Tema/JRPG/BattleUI/PartyHoveredWidget.h"
#include "Tema/JRPG/MainUI/JRPGSettingPartySlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void UJRPGSettingPartyIcon::Init(class UTexture2D* Texture)
{
	if (Texture && OwnerField)
	{
		CharIcon->SetBrushFromTexture(Texture);
		Button->OnClicked.AddDynamic(this, &UJRPGSettingPartyIcon::SetPartyList);
		// ��ư Ŭ���� PartyField�� SetSpawnUnit�� �����ϰ�, �ش� ĭ�� ������ �ٽ� �Ѵ�.
		// �߿��Ѱ��� ���� 1 2 3�� ĭ�� 1��ĭ�� ��� 2�� 3���� 1 �� 2���� �Ǵ� ������ �ؾ��Ѵ�.
		// �Ǵ� ������ ĳ���Ͱ� �ش� ��Ƽ�� ������� ���� �ڸ��� �ٲ���Ѵ�.
		// �̹� ��Ƽâ�� ��ϵǾ��ִ� ĳ���͵��� ǥ�� �ǰ� �Ѵ�.

		Button->OnHovered.AddDynamic(this, &UJRPGSettingPartyIcon::SetHoveredWidget);
		Button->OnUnhovered.AddDynamic(this, &UJRPGSettingPartyIcon::SetUnHoveredWidget);
	}
}



void UJRPGSettingPartyIcon::SetPartyList()
{
	OwnerField->SetPartyList(CharNum, this);
	if (Button->BackgroundColor != SelectColor)
	{
		Button->SetBackgroundColor(SelectColor);
		if (HoveredWidget)
		{
			if (HoveredWidget->IsInViewport())
				HoveredWidget->RemoveFromParent();

			OwnerWidget->HoveredWidget = nullptr;
			HoveredWidget = nullptr;
		}
	}
}


void UJRPGSettingPartyIcon::SetSelectColor(bool bFlag)
{
	if (bFlag) // true���� false���� HoriList���� �Ǵ�.
	{
		Button->SetBackgroundColor(SelectColor);
	}
	else
	{
		Button->SetBackgroundColor(NormalColor);
	}
}


void UJRPGSettingPartyIcon::SetHoveredWidget()
{
	if (BP_HoveredWidget)
	{
		if (HoveredWidget == nullptr)
		{
			HoveredWidget = CreateWidget<UPartyHoveredWidget>(GetWorld(), BP_HoveredWidget);
			HoveredWidget->OwnerController = OwnerController;
			HoveredWidget->CharNum = CharNum;
			HoveredWidget->Init();
			OwnerWidget->HoveredWidget = HoveredWidget;
			// �̷��� �Ѱ��ְ� �ֻ��� ������ Tick���� AddToViewport�Ѵ�.
		}
	}
}

void UJRPGSettingPartyIcon::SetUnHoveredWidget()
{
	if (BP_HoveredWidget)
	{
		if (!HoveredWidget)
			return;

		if (HoveredWidget->IsInViewport())
		{
			HoveredWidget->RemoveFromParent();
			OwnerWidget->HoveredWidget = nullptr;
			HoveredWidget = nullptr;		
		}
	}
}
