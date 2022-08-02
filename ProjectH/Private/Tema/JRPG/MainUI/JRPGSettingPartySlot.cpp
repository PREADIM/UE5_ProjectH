// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGSettingPartySlot.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Components/ScrollBox.h"
#include "Tema/JRPG/MainUI/JRPGSettingHoriList.h"
#include "Tema/JRPG/CustomWidget.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"



void UJRPGSettingPartySlot::Init()
{
	// ���⼭ HoriList�� �����ؼ� �� ����Ʈ�ȿ��ִ� Icon�� �����Ͽ� Add�ϰ� �װ��� Scroll�� Add�Ѵ�.

	if (OwnerController)
	{
		HaveCharNum = (OwnerController->HaveCharList.Num() / 4) + 1; // +1 �� �� ����.
		Exit->OnClicked.AddDynamic(this, &UJRPGSettingPartySlot::ExitWidget);
		SetList();
	}
}


void UJRPGSettingPartySlot::SetList()
{
	if (!BP_HoriList)
		return;
	int32 num = (OwnerController->HaveCharList.Num() / 4) + 1;
	Scroll->ClearChildren();

	for (int32 Row = 0; Row < num; Row++)
	{
		UJRPGSettingHoriList* HoriList = CreateWidget<UJRPGSettingHoriList>(GetWorld(), BP_HoriList);
		if (HoriList)
		{
			HoriList->OwnerController = OwnerController;
			HoriList->OwnerField = OwnerField;

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



void UJRPGSettingPartySlot::SetCloseFunction()
{
	ExitWidget();
}


void UJRPGSettingPartySlot::ExitWidget()
{
	if (OwnerField)
	{
		RemoveFromParent();
		OwnerController->LastWidget.Pop();
		OwnerField->ResomeUI();
	}
}