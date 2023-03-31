// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/SkillDetailWidget.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"



void USkillDetailWidget::Init(FString Name, FString Detail)
{
	AttackName->SetText(FText::FromString(Name));
	RichAttackDetail->SetText(FText::FromString(Detail));
}