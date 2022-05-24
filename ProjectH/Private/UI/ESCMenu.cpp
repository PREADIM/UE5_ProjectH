// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ESCMenu.h"
#include "Components/Button.h"
#include "UI/OptionMenu.h"
#include "Components/CanvasPanel.h"

/*void UESCMenu::NativeConstruct()
{

}*/

void UESCMenu::Init()
{
	Resome->OnClicked.AddDynamic(this, &UESCMenu::ResomeClick);
	Setting->OnClicked.AddDynamic(this, &UESCMenu::SettingClick);
	Quit->OnClicked.AddDynamic(this, &UESCMenu::QuitClick);

	if (BP_OptionMenu != nullptr)
	{
		OptionMenu = CreateWidget<UOptionMenu>(OwnerController, BP_OptionMenu);
		OptionMenu->Init();
		OptionMenu->PrevButton->OnClicked.AddDynamic(this, &UESCMenu::PrevClick);
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

void UESCMenu::QuitClick()
{
	// ���� ����. Ÿ��Ʋ�� ���ư���. Ÿ��Ʋ ����� �ؽ�Ʈ ���� �ϸ�ɵ�.
	//BlueprintImplemetableEvent�� �������Ʈ �ȿ��� ���� �ɵ��ϴ�.
	//�ƿ� ���ε��� �ű⼭ �ص� �ɵ�.
}


void UESCMenu::PrevClick()
{
	OptionMenu->OptionAnimation(true);
	ESCMenuAnimation(false);
	// Setting Click �ݴ�� Ű��
}

void UESCMenu::SetMouseOff()
{
	if(OwnerController)
	{
		OwnerController->SetShowMouseCursor(false);
		OwnerController->SetInputMode(FInputModeGameOnly());
	}
	
}

//true�� ���� �Ű�, false�� Ű�� ���̴�.
void UESCMenu::ESCMenuAnimation(bool IsOpened)
{
	PlayAnimation(ESCMenuFade, 0.f, 1, IsOpened ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
	SetVisibility(IsOpened ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
}


