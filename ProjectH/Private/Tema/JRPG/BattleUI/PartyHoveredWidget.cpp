// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/PartyHoveredWidget.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Components/TextBlock.h"
#include "Tema/JRPG/JRPGUnit.h"


void UPartyHoveredWidget::Init()
{
	if (OwnerController)
	{
	
		if (OwnerController->HaveCharStat.Find(CharNum) != nullptr)
		{

			FJRPGCharStat Stat = OwnerController->GetCharStat(CharNum);

			CharName->SetText(FText::FromString(OwnerController->GetUnitUI(CharNum)->CharName));
			CharLevel->SetText(FText::FromString(FString::FromInt(Stat.CharLevel)));
			CharHP->SetText(FText::FromString(FString::FromInt(Stat.MaxHP)));
			CharMP->SetText(FText::FromString(FString::FromInt(Stat.MaxMP)));
			CharAttack->SetText(FText::FromString(FString::FromInt(Stat.Attack)));
			CharAttack->SetText(FText::FromString(FString::FromInt(Stat.Shelid)));
			MaxULT->SetText(FText::FromString(FString::FromInt(Stat.MaxULT)));
		}
	
	}
}


