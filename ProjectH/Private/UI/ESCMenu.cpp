// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ESCMenu.h"
#include "Components/Button.h"
#include "UI/OptionMenu.h"
#include "UI/HelpMenu.h"
#include "Components/CanvasPanel.h"

/*void UESCMenu::NativeConstruct()
{

}*/

void UESCMenu::Init()
{
	Resome->OnClicked.AddDynamic(this, &UESCMenu::ResomeClick);
	Setting->OnClicked.AddDynamic(this, &UESCMenu::SettingClick);
	Help->OnClicked.AddDynamic(this, &UESCMenu::HelpClick);
	Quit->OnClicked.AddDynamic(this, &UESCMenu::QuitClick);

	if (BP_OptionMenu != nullptr)
	{
		OptionMenu = CreateWidget<UOptionMenu>(OwnerController, BP_OptionMenu);
		OptionMenu->Init();
		OptionMenu->OwnerController = OwnerController;
		OptionMenu->PrevButton->OnClicked.AddDynamic(this, &UESCMenu::PrevClick);
	}
	if (BP_HelpMenu != nullptr)
	{
		HelpMenu = CreateWidget<UHelpMenu>(OwnerController, BP_HelpMenu);
		HelpMenu->OwnerController = OwnerController;
		HelpMenu->PrevButton->OnClicked.AddDynamic(this, &UESCMenu::HelpUIOff);
	}

}



void UESCMenu::ResomeClick()
{
	ESCMenuAnimation(true);
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UESCMenu::SetMouseOff, ESCMenuFade->GetEndTime(), false);
}

void UESCMenu::SettingClick()
{
	// false�� �ѱ�.
	if (OptionMenu)
	{
		OptionMenu->AddToViewport();
		OptionMenu->SetComboBox();
		OptionMenu->OptionAnimation(false);
		ESCMenuAnimation(true); // �ɼ��� ų���� ��Ʈ�׽�Ʈ�� �׹�° �κ����� �̿�����.
	}
}

void UESCMenu::HelpClick()
{
	if (HelpMenu)
	{
		HelpMenu->AddToViewport();
		HelpMenu->HelpUIAnim(false);
		ESCMenuAnimation(true);
	}
}

void UESCMenu::QuitClick()
{
	// ���� ����. Ÿ��Ʋ�� ���ư���. Ÿ��Ʋ ����� �ؽ�Ʈ ���� �ϸ�ɵ�.
	//BlueprintImplemetableEvent�� �������Ʈ �ȿ��� ���� �ɵ��ϴ�.
	//�ƿ� ���ε��� �ű⼭ �ص� �ɵ�.
}


void UESCMenu::PrevClick()
{
	OptionMenu->OptionAnimation(true);
	if (bOptionKeyOpen) // ESC �޴��� ��ġ�� �ʰ�, Ű ����Ű�� �ɼ�â�� ������.
	{
		OptionMenu->OptionAnimation(true);
		OwnerController->SetShowMouseCursor(false);
		OwnerController->SetInputMode(FInputModeGameOnly());
		bOptionKeyOpen = false;
	}
	else
	{
		ESCMenuAnimation(false);
		// Setting Click �ݴ�� Ű��
	}
}


/* Ű���� ����Ű�� UI�� �����Ҷ�. */
bool UESCMenu::SettingKeyClick()
{
	if (OptionMenu)
	{
		if (OptionMenu->IsInViewport())
		{
			OptionMenu->OptionAnimation(true);
			bOptionKeyOpen = false;
			return false;
		}
		else
		{
			OptionMenu->AddToViewport();
			OptionMenu->SetComboBox();
			OptionMenu->OptionAnimation(false);
			bOptionKeyOpen = true;
			return true;
		}
	}

	return false; // �ϴ� ��ȯ�� ������ ���Ŀ� ������ ���� ���ɼ� ����.
	// true�� ���� Ų ��, false�� ���� �� ��.
}


void UESCMenu::SetMouseOff()
{
	if(OwnerController)
	{
		OwnerController->SetShowMouseCursor(false);
		OwnerController->SetInputMode(FInputModeGameOnly());
	}
}


void UESCMenu::HelpUIOff()
{
	HelpMenu->HelpUIAnim(true);
	SetMouseOff();
}


//true�� ���� �Ű�, false�� Ű�� ���̴�.
void UESCMenu::ESCMenuAnimation(bool IsOpened)
{
	PlayAnimation(ESCMenuFade, 0.f, 1, IsOpened ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
	SetVisibility(IsOpened ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
}


