// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGULTButton.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UJRPGULTButton::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UJRPGULTButton::Init()
{
	if (GM)
	{
		CurrentUnit = GM->SetUnitList[0].Unit;
		ULTGage = CurrentUnit->CurrentULTGage;
		MaxULTGage = CurrentUnit->MaxULTGage;
		FProgressBarStyle Style;
		FSlateBrush SlateWidget = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.ULT.ULTImg);
		FSlateBrush FillWidget = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.ULT.FillULTImg);
		Style.SetBackgroundImage(SlateWidget);
		Style.SetFillImage(FillWidget);

		PB_ULT->SetWidgetStyle(Style);
		PB_ULT->PercentDelegate.BindUFunction(this, "BindULTGage");
		PB_ULT->SynchronizeProperties();

		ULTButtonSetActive();
		if (!ULTButton->OnClicked.IsBound())
			ULTButton->OnClicked.AddDynamic(this, &UJRPGULTButton::UseSkill);

	}
}


void UJRPGULTButton::ULTButtonSetActive()
{
	if (CurrentUnit->MaxULTGage <= ULTGage)
		ULTButton->SetIsEnabled(true);
	else
		ULTButton->SetIsEnabled(false);
}


void UJRPGULTButton::UseSkill()
{
	if (CurrentUnit)
		CurrentUnit->Skill_ULT();
}

float UJRPGULTButton::BindULTGage()
{
	return ULTGage / MaxULTGage;
}



//void UJRPGULTButton::EnermyTurnFirstInit()
//{
//	if (GM)
//	{
//		CurrentUnit = GM->OwnerUnits.HeapTop().Unit;
//		ULTGage = CurrentUnit->CurrentULTGage;
//		FProgressBarStyle Style;
//		FSlateBrush SlateWidget = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.ULT.ULTImg);
//		FSlateBrush FillWidget = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.ULT.FillULTImg);
//		Style.SetBackgroundImage(SlateWidget);
//		Style.SetFillImage(SlateWidget);
//
//		PB_ULT->SetWidgetStyle(Style);
//
//		ULTButton->OnClicked.AddDynamic(this, &UJRPGULTButton::UseSkill);
//	}
//}


