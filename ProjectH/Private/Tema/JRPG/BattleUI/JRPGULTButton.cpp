// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGULTButton.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UJRPGULTButton::Init()
{
	if (GM)
	{
		CurrentUnit = GM->UnitList[0].Unit;
		ULTGage = CurrentUnit->ULTGage;
		FProgressBarStyle Style;
		FSlateBrush SlateWidget = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.ULT.ULTImg);
		FSlateBrush FillWidget = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.ULT.FillULTImg);
		Style.SetBackgroundImage(SlateWidget);
		Style.SetFillImage(SlateWidget);

		PB_ULT->WidgetStyle = Style;
		if (CurrentUnit->MaxULTGage <= ULTGage)
			bULT = true;
		else
			bULT = false;

		ULTButton->OnClicked.AddDynamic(this, &UJRPGULTButton::UseSkill);
	}
}


void UJRPGULTButton::UseSkill()
{
	if (CurrentUnit)
	{
		if (CurrentUnit->MaxULTGage <= ULTGage)
		{
			CurrentUnit->CallULT();
		}
	}

}


void UJRPGULTButton::EnermyTurnFirstInit()
{
	if (GM)
	{
		CurrentUnit = GM->OwnerUnits[0].Unit;
		ULTGage = CurrentUnit->ULTGage;
		FProgressBarStyle Style;
		FSlateBrush SlateWidget = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.ULT.ULTImg);
		FSlateBrush FillWidget = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.ULT.FillULTImg);
		Style.SetBackgroundImage(SlateWidget);
		Style.SetFillImage(SlateWidget);

		PB_ULT->WidgetStyle = Style;
		if (CurrentUnit->MaxULTGage <= ULTGage)
			bULT = true;
		else
			bULT = false;

		ULTButton->OnClicked.AddDynamic(this, &UJRPGULTButton::UseSkill);
	}
}