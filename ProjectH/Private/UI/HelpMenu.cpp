// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HelpMenu.h"
#include "Components/Button.h"


void UHelpMenu::Init()
{

}


void UHelpMenu::HelpUIAnim(bool IsOpened)
{
	PlayAnimation(HelpAnimation, 0.f, 1, IsOpened ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
	if (IsOpened)
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &UHelpMenu::HelpMenuRemove, HelpAnimation->GetEndTime(), false);
	}
}


void UHelpMenu::HelpMenuRemove()
{
	RemoveFromParent();
}


void UHelpMenu::InputMode()
{
	if (OwnerController)
	{
		OwnerController->SetInputMode(FInputModeUIOnly());
	}
}