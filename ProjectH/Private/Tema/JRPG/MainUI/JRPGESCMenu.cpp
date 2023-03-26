// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGESCMenu.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/MainUI/JRPGPartySettingUI.h"
#include "Tema/JRPG/MainUI/JRPGMainWidget.h"
#include "Components/Button.h"
#include "Tema/JRPG/CustomWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"

void UJRPGESCMenu::Init()
{
	Resome->OnClicked.AddDynamic(this, &UJRPGESCMenu::ResomeFunc);
	PartySetting->OnClicked.AddDynamic(this, &UJRPGESCMenu::PartySet);
	Quit->OnClicked.AddDynamic(this, &UJRPGESCMenu::QuitTema);

}


// 파티설정 UI키기
void UJRPGESCMenu::PartySet()
{
	if (!OwnerController || !BP_PartyField)
		return;

	if (OwnerMainUI)
		SpawnPartyField();
}

void UJRPGESCMenu::SetCloseFunction()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	OwnerMainUI->ReverseESC();
}


void UJRPGESCMenu::ResomeFunc()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	OwnerMainUI->ReverseESC();
}

void UJRPGESCMenu::SpawnPartyField()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);

	if (BP_PartyField)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		PartyField = GetWorld()->SpawnActor<APartySettingField>(BP_PartyField, FTransform(FVector(7441.f, -14870.f, 9812.f)), SpawnParameters);
		if (PartyField)
		{
			PartyField->OwnerController = OwnerController;
			PartyField->Init(OwnerMainUI);
			OwnerController->OnPossess(Cast<APawn>(PartyField));
			OwnerController->MouseOn();
			OwnerMainUI->SetVisibility(ESlateVisibility::Hidden);		
		}
	}
}

// 테마 나가기.
void UJRPGESCMenu::QuitTema()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);

	if (OwnerMainUI && BP_QuitWidget)
	{	
		QuitWidget = CreateWidget<UUserWidget>(GetWorld(), BP_QuitWidget);
		if (QuitWidget)
		{

			OwnerMainUI->ReverseESC();
			OwnerController->MouseOn();
			QuitWidget->AddToViewport();
		}
	}
}

