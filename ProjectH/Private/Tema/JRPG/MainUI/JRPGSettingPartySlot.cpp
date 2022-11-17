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
	// ���⼭ HoriList�� �����ؼ� �� ����Ʈ�ȿ��ִ� Icon�� �����Ͽ� Add�ϰ� �װ��� Scroll�� Add�Ѵ�.

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
	int32 num = (OwnerController->HaveCharList.Num() / 4) + 1; // +1 �� �� ����.
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
	// ���� ���õ� ĭ ����.
	ExitWidget();
	OwnerField->SpawnCharacter(); // �ٽ� ���Ӱ� ����.
	SetCurrentPartySave();
}

void UJRPGSettingPartySlot::SetPartyChange()
{
	for (int32 i = 0; i < OwnerController->CurrentParty.Num(); i++)
	{
		if (OwnerController->CurrentParty[i] == SetCharNum)
		{
			// �̹� ���� ���� �����Ѵ�.
			int32 temp = OwnerController->CurrentParty[i];
			OwnerController->CurrentParty[i] = OwnerController->CurrentParty[SelectFieldNumber];
			OwnerController->CurrentParty[SelectFieldNumber] = temp;
			ExitWidget();
			OwnerField->SpawnCharacter(); // �ٽ� ���Ӱ� ����.
			SetCurrentPartySave();
			return;
		}
	}

	// ����������� �ƿ� ��� �߰��ϴ� ��.
	if (OwnerController->CurrentParty.IsValidIndex(SelectFieldNumber))
	{
		// ���𰡰� �����Ѵ�. (ĳ���� ��ü)
		OwnerController->CurrentParty[SelectFieldNumber] = SetCharNum;
	}
	else // �ƿ� ��ĭ.
	{
		OwnerController->CurrentParty.Add(SetCharNum);
	}

	ExitWidget();
	OwnerField->SpawnCharacter(); // �ٽ� ���Ӱ� ����.
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