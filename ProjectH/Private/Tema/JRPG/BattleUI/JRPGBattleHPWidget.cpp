// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGBattleHPWidget.h"
#include "Components/ProgressBar.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Components/TextBlock.h"

void UJRPGBattleHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HP->PercentDelegate.BindUFunction(this, "RetHP");
	MP->PercentDelegate.BindUFunction(this, "RetMP");
	ULT->PercentDelegate.BindUFunction(this, "RetULT");
	HP->SynchronizeProperties();
	MP->SynchronizeProperties();
	ULT->SynchronizeProperties();

}

void UJRPGBattleHPWidget::Init()
{
	CharLevel->SetText(FText::FromString(FString::FromInt(OwnerUnit->CharacterStat.CharLevel)));
}

float UJRPGBattleHPWidget::RetHP()
{
	if (!OwnerUnit)
		return 0.0f;

	return OwnerUnit->CurrentHP / OwnerUnit->CharacterStat.MaxHP;
}

float UJRPGBattleHPWidget::RetMP()
{
	if (!OwnerUnit)
		return 0.0f;

	return OwnerUnit->CurrentMP / OwnerUnit->CharacterStat.MaxMP;
}


float UJRPGBattleHPWidget::RetULT()
{
	if (!OwnerUnit)
		return 0.0f;

	return  OwnerUnit->ULTGage / OwnerUnit->MaxULTGage;
}
