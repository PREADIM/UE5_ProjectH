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
		/*switch (ButtonType)
		{
		case EButtonType::NormalAttack :
			CurrentUnit = GM->UnitList.HeapTop().Unit;
			CostMP = 0;
			SetButton(CurrentUnit->UnitSkills.NormalAttack.SkillImg);
			break;
		case EButtonType::SkillAttack :
			CurrentUnit = GM->UnitList.HeapTop().Unit;
			CostMP = CurrentUnit->UnitSkills.Skill_1.CostMP;
			SetButton(CurrentUnit->UnitSkills.Skill_1.SkillImg);		
			SkillSetActive();
			break;
		}*/

		switch (ButtonType)
		{
		case EButtonType::NormalAttack:
			CurrentUnit = GM->SetUnitList[0].Unit;
			CostMP = 0;
			SetButton(CurrentUnit->UnitSkills.NormalAttack.SkillImg);
			break;
		case EButtonType::SkillAttack:
			CurrentUnit = GM->SetUnitList[0].Unit;
			CostMP = CurrentUnit->UnitSkills.Skill_1.CostMP;
			SetButton(CurrentUnit->UnitSkills.Skill_1.SkillImg);
			SkillSetActive();
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
			CurrentUnit = GM->OwnerUnits.HeapTop().Unit;
			CostMP = 0;
			SetButton(CurrentUnit->UnitSkills.NormalAttack.SkillImg);
			break;
		case EButtonType::SkillAttack:
			CurrentUnit = GM->OwnerUnits.HeapTop().Unit;
			CostMP = CurrentUnit->UnitSkills.Skill_1.CostMP;
			SetButton(CurrentUnit->UnitSkills.Skill_1.SkillImg);
			SkillSetActive();
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
			CurrentUnit->NormalAttack();
			break;
		case EButtonType::SkillAttack:
			if (CurrentUnit->CurrentMP >= CostMP)
			{
				CurrentUnit->Skill_1();
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

void UJRPGSkillButton::SetButton(UTexture2D* Tex)
{
	FSlateBrush Icon;
	Icon = UWidgetBlueprintLibrary::MakeBrushFromTexture(Tex);
	ButtonSetImage(Icon);
	if (!SkillButton->OnClicked.IsBound())
	{
		SkillButton->OnClicked.AddDynamic(this, &UJRPGSkillButton::UseSkill);
	}
}
