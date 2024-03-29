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
		// 버튼 클릭에 PartyField의 SetSpawnUnit을 연동하고, 해당 칸의 스폰을 다시 한다.
		// 중요한것은 만약 1 2 3번 칸의 1번칸이 비면 2번 3번이 1 번 2번이 되는 식으로 해야한다.
		// 또는 선택한 캐릭터가 해당 파티에 있을경우 서로 자리를 바꿔야한다.
		// 이미 파티창에 등록되어있는 캐릭터들은 표시 되게 한다.

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
	if (bFlag) // true인지 false인지 HoriList에서 판단.
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
			// 이렇게 넘겨주고 최상위 위젯의 Tick에서 AddToViewport한다.
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
