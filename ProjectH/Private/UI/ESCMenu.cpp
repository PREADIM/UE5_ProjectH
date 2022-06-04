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
	// false가 켜기.
	if (OptionMenu)
	{
		OptionMenu->AddToViewport();
		OptionMenu->SetComboBox();
		OptionMenu->OptionAnimation(false);
		ESCMenuAnimation(true); // 옵션을 킬때는 히트테스트가 네번째 인비지블 이여야함.
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
	// 게임 종료. 타이틀로 돌아가기. 타이틀 만들면 넥스트 레벨 하면될듯.
	//BlueprintImplemetableEvent로 블루프린트 안에서 만들어도 될듯하다.
	//아예 바인딩도 거기서 해도 될듯.
}


void UESCMenu::PrevClick()
{
	OptionMenu->OptionAnimation(true);
	if (bOptionKeyOpen) // ESC 메뉴를 거치지 않고, 키 단축키로 옵션창을 열었다.
	{
		OptionMenu->OptionAnimation(true);
		OwnerController->SetShowMouseCursor(false);
		OwnerController->SetInputMode(FInputModeGameOnly());
		bOptionKeyOpen = false;
	}
	else
	{
		ESCMenuAnimation(false);
		// Setting Click 반대로 키기
	}
}


/* 키보드 단축키로 UI를 실행할때. */
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

	return false; // 일단 반환은 하지만 추후에 문제가 생길 가능성 있음.
	// true면 이제 킨 것, false면 이제 끈 것.
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


//true면 끄는 거고, false면 키는 것이다.
void UESCMenu::ESCMenuAnimation(bool IsOpened)
{
	PlayAnimation(ESCMenuFade, 0.f, 1, IsOpened ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
	SetVisibility(IsOpened ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
}


