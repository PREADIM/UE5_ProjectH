// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGSkillButton.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UJRPGSkillButton::Init()
{
	if (GM)
	{
		FSlateBrush Icon;

		switch (ButtonType)
		{
		case EButtonType::NormalAttack :
			CurrentUnit = GM->UnitList[0].Unit;
			Icon = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.NormalAttack.SkillImg);
			CostMP = 0;
			ButtonSetImage(Icon);
			SkillButton->OnClicked.AddDynamic(this, &UJRPGSkillButton::UseSkill);
			break;
		case EButtonType::SkillAttack :
			CurrentUnit = GM->UnitList[0].Unit;
			Icon = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.Skill_1.SkillImg);
			CostMP = CurrentUnit->UnitSkills.Skill_1.CostMP;
			ButtonSetImage(Icon);
			SkillSetActive();
			SkillButton->OnClicked.AddDynamic(this, &UJRPGSkillButton::UseSkill);
			break;
		}
	}
}

void UJRPGSkillButton::EnermyTurnFirstInit()
{
	if (GM)
	{
		FSlateBrush Icon;

		switch (ButtonType)
		{
		case EButtonType::NormalAttack:
			CurrentUnit = GM->OwnerUnits[0].Unit;
			CostMP = 0;
			Icon = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.NormalAttack.SkillImg);
			ButtonSetImage(Icon);
			SkillButton->OnClicked.AddDynamic(this, &UJRPGSkillButton::UseSkill);
			break;
		case EButtonType::SkillAttack:
			CurrentUnit = GM->OwnerUnits[0].Unit;
			CostMP = CurrentUnit->UnitSkills.Skill_1.CostMP;
			Icon = UWidgetBlueprintLibrary::MakeBrushFromTexture(CurrentUnit->UnitSkills.Skill_1.SkillImg);
			ButtonSetImage(Icon);
			SkillButton->OnClicked.AddDynamic(this, &UJRPGSkillButton::UseSkill);
			break;
		}
	}
}

void UJRPGSkillButton::UseSkill()
{
	if (CurrentUnit)
	{
		switch (ButtonType)
		{
		case EButtonType::NormalAttack:
			CurrentUnit->CallNormalAttack();
			break;
		case EButtonType::SkillAttack:
			if (CurrentUnit->CurrentMP >= CostMP)
			{
				CurrentUnit->CallSkill_1();
			}
			break;
		}
	}

}


void UJRPGSkillButton::ButtonSetImage(FSlateBrush Icon)
{
	FButtonStyle Style;
	Style.SetNormal(Icon);
	Style.Normal.TintColor = FSlateColor(NormalColor);

	Style.SetHovered(Icon);
	Style.Hovered.TintColor = FSlateColor(HoveredColor);

	Style.SetPressed(Icon);
	Style.Pressed.TintColor = FSlateColor(PressedColor);

	Style.SetDisabled(Icon);
	Style.Disabled.TintColor = FSlateColor(DisableColor);

	SkillButton->WidgetStyle = Style;
}



void UJRPGSkillButton::SkillSetActive()
{
	if (CurrentUnit->CurrentMP < CostMP)
	{
		SkillButton->SetIsEnabled(false);
	}
	else
		SkillButton->SetIsEnabled(true);
}