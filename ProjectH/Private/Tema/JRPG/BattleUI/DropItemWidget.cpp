// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/DropItemWidget.h"
#include "Components/TextBlock.h"
#include "Tema/JRPG/JRPGPlayerController.h"

void UDropItemWidget::Init(FString CharName)
{
	DropCharName->SetText(FText::FromString(CharName));
}

void UDropItemWidget::MouseOff()
{
	if (OwnerController)
	{
		OwnerController->MouseOff();
	}
}

