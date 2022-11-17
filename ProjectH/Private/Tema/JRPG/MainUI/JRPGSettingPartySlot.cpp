// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGSettingPartySlot.h"
#include "Tema/JRPG/BattleUI/PartyHoveredWidget.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Components/ScrollBox.h"
#include "Tema/JRPG/MainUI/JRPGSettingHoriList.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Blueprint/WidgetLayoutLibrary.h"



void UJRPGSettingPartySlot::Init()
{
	// 여기서 HoriList를 생성해서 그 리스트안에있는 Icon을 셋팅하여 Add하고 그것을 Scroll에 Add한다.

	if (OwnerController)
	{
		Exit->OnClicked.AddDynamic(this, &UJRPGSettingPartySlot::ExitWidget);
		Out->OnClicked.AddDynamic(this, &UJRPGSettingPartySlot::SetPartyOut);
		Select->OnClicked.AddDynamic(this, &UJRPGSettingPartySlot::SetPartyChange);
		//Arrow_L->OnClicked.AddDynamic(this, &UJRPGSettingPartySlot::ArrowLeftClick);
		//Arrow_R->OnClicked.AddDynamic(this, &UJRPGSettingPartySlot::ArrowRightClick);
	}
}


void UJRPGSettingPartySlot::SetList()
{
	if (!BP_HoriList)
		return;
	int32 num = (OwnerController->HaveCharList.Num() / 4) + 1; // +1 을 꼭 하자.
	Scroll->ClearChildren();

	for (int32 Row = 0; Row < num; Row++)
	{
		UJRPGSettingHoriList* HoriList = CreateWidget<UJRPGSettingHoriList>(GetWorld(), BP_HoriList);
		HoriList->OwnerWidget = this;
		if (HoriList)
		{
			HoriList->OwnerController = OwnerController;
			HoriList->OwnerField = OwnerField;
			HoriList->OwnerWidget = this;

			int32 cnt = 0;
			for (int32 i = Row * 4; OwnerController->HaveCharList.IsValidIndex(i) && cnt < 4; i++)
			{
				HoriList->CharNums.Add(OwnerController->HaveCharList[i]);
				cnt++;
			}

			HoriList->Init();
			HoriList->SetPadding(FMargin(0.f, 10.f, 0.f, 0.f));
			Scroll->AddChild(HoriList);
		}
	}
}

void UJRPGSettingPartySlot::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (HoveredWidget)
	{	
		HoveredWidget->SetRenderTranslation(UWidgetLayoutLibrary::GetMousePositionOnViewport(this) + FVector2D(5.f, 5.f));
		if(!HoveredWidget->IsInViewport())
			HoveredWidget->AddToViewport();
	}
}



void UJRPGSettingPartySlot::SetCloseFunction()
{
	ExitWidget();
}

void UJRPGSettingPartySlot::SetVisibilityOutButton(bool bFlag)
{
	if (bFlag)
	{
		if (Out->GetVisibility() != ESlateVisibility::Visible)
			Out->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (Out->GetVisibility() != ESlateVisibility::Hidden)
			Out->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UJRPGSettingPartySlot::SetVisibilitySelectButton(bool bFlag)
{
	if (bFlag)
	{
		if(Select->GetVisibility() != ESlateVisibility::Visible)
			Select->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (Select->GetVisibility() != ESlateVisibility::Hidden)
			Select->SetVisibility(ESlateVisibility::Hidden);
	}

}


void UJRPGSettingPartySlot::ExitWidget()
{
	if (OwnerField)
	{
		RemoveFromParent();
		if (HoveredWidget)
		{
			if (HoveredWidget->IsInViewport())
			{
				HoveredWidget->RemoveFromParent();
				HoveredWidget = nullptr;
			}
		}
		OwnerController->LastWidget.Pop();
		OwnerField->ResomeUI();
	}
}

void UJRPGSettingPartySlot::SetPartyOut()
{
	OwnerController->CurrentParty.RemoveAt(SelectFieldNumber);
	// 현재 선택된 칸 해제.
	ExitWidget();
	OwnerField->SpawnCharacter(); // 다시 새롭게 스폰.
	SetCurrentPartySave();
}

void UJRPGSettingPartySlot::SetPartyChange()
{
	for (int32 i = 0; i < OwnerController->CurrentParty.Num(); i++)
	{
		if (OwnerController->CurrentParty[i] == SetCharNum)
		{
			// 이미 같은 것이 존재한다.
			int32 temp = OwnerController->CurrentParty[i];
			OwnerController->CurrentParty[i] = OwnerController->CurrentParty[SelectFieldNumber];
			OwnerController->CurrentParty[SelectFieldNumber] = temp;
			ExitWidget();
			OwnerField->SpawnCharacter(); // 다시 새롭게 스폰.
			SetCurrentPartySave();
			return;
		}
	}

	// 여기까지오면 아예 없어서 추가하는 것.
	if (OwnerController->CurrentParty.IsValidIndex(SelectFieldNumber))
	{
		// 무언가가 존재한다. (캐릭터 교체)
		OwnerController->CurrentParty[SelectFieldNumber] = SetCharNum;
	}
	else // 아예 빈칸.
	{
		OwnerController->CurrentParty.Add(SetCharNum);
	}

	ExitWidget();
	OwnerField->SpawnCharacter(); // 다시 새롭게 스폰.
	SetCurrentPartySave();
}

void UJRPGSettingPartySlot::ArrowLeftClick()
{
	if (OwnerField)
	{
		OwnerField->NextChar(-1);
	}

}

void UJRPGSettingPartySlot::ArrowRightClick()
{
	if (OwnerField)
	{
		OwnerField->NextChar(1);
	}
}


void UJRPGSettingPartySlot::SetCurrentPartySave()
{
	if (GM)
	{
		GM->SetSaveJRPG();
		OwnerController->SetPartyChange();
	}
}


void UJRPGSettingPartySlot::PlayWarningText()
{
	PlayAnimation(PlayWarning, 0.f, 1, EUMGSequencePlayMode::Forward);

}