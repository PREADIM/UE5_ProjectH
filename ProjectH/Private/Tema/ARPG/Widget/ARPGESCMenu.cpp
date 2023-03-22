
#include "Tema/ARPG/Widget/ARPGESCMenu.h"
#include "PlayerControllerBase.h"
#include "Components/Button.h"
#include "UI/OptionMenu.h"


void UARPGESCMenu::Init(class AARPGUnitBase* Unit)
{
	Super::Init(Unit);

	if (BP_QuitWidget)
		QuitWidget = CreateWidget<UUserWidget>(GetWorld(), BP_QuitWidget);

	if (BP_OptionMenu)
	{
		OptionMenu = CreateWidget<UOptionMenu>(GetWorld(), BP_OptionMenu);
		OptionMenu->OwnerController = OwnerController;
		OptionMenu->Init();
	}
		
	Resome->OnClicked.AddDynamic(this, &UARPGESCMenu::ResomeFunc);
	Option->OnClicked.AddDynamic(this, &UARPGESCMenu::OptionFunc);
	Quit->OnClicked.AddDynamic(this, &UARPGESCMenu::QuitTema);
	OptionMenu->PrevButton->OnClicked.AddDynamic(this, &UARPGESCMenu::OptionPrevButton);

}

void UARPGESCMenu::ResomeFunc()
{
	PlayESCAnim(true);
}

void UARPGESCMenu::OptionFunc()
{
	if (OptionMenu)
	{
		RemoveFromParent();
		bESCMenuOpen = false;		
		OptionMenu->AddViewportSetupComboBox();
		OptionMenu->OptionAnimation(false);
	}
}

void UARPGESCMenu::QuitTema()
{
	RemoveFromParent();
	bESCMenuOpen = false;

	if (QuitWidget)
		QuitWidget->AddToViewport();
}


void UARPGESCMenu::ESCClick()
{
	if (OptionMenu->IsInViewport())
	{
		OwnerController->MouseOff();
		OptionMenu->OptionAnimation(true);
		return;
	}


	if (!IsPlayingAnimation())
		PlayESCAnim(bESCMenuOpen);
}


/* true : ²ô±â /  false : ÄÑ±â */
void UARPGESCMenu::PlayESCAnim(bool bFlag)
{
	PlayAnimation(ESCAnim, 0.f, 1, bFlag ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
	if (bFlag)
	{
		OwnerController->MouseOff();
		bESCMenuOpen = false;
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &UARPGESCMenu::ESCRemove, ESCAnim->GetEndTime(), false);
	}
	else
	{
		AddToViewport();
		bESCMenuOpen = true;
		OwnerController->MouseOn();
	}
}


void UARPGESCMenu::ESCRemove()
{
	if (IsInViewport())
		RemoveFromParent();		
}


void UARPGESCMenu::OptionPrevButton()
{
	OptionMenu->OptionAnimation(true);
	PlayESCAnim(false);
}