// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGBattleHPWidget.h"
#include "Components/ProgressBar.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Tema/JRPG/BattleUI/JRPGBuffWidget.h"

void UJRPGBattleHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HP->SynchronizeProperties();
	MP->SynchronizeProperties();
	ULT->SynchronizeProperties();

}

void UJRPGBattleHPWidget::Init(AJRPGUnit* Unit)
{
	OwnerUnit = Unit;
	HP->PercentDelegate.BindUFunction(this, "RetHP");
	MP->PercentDelegate.BindUFunction(this, "RetMP");
	ULT->PercentDelegate.BindUFunction(this, "RetULT");
	CharLevel->SetText(FText::FromString(FString::FromInt(OwnerUnit->CharacterStat.CharLevel)));
	
}

void UJRPGBattleHPWidget::SetBuffIcon()
{
	BuffWidgets.Empty();
	BuffIconWrapBox->ClearChildren();
	for (FDebuffStruct DebuffStruct : OwnerUnit->DebuffSet)
	{
		UJRPGBuffWidget* BuffWidget = CreateWidget<UJRPGBuffWidget>(GetWorld(), BP_BuffWidget);
		if (BuffWidget)
		{
			BuffWidget->Init(DebuffStruct.DebuffClass, EIconSizeType::Midium);
			BuffWidget->SetPadding(FMargin(0.f, 0.f, 3.f, 3.f));
			BuffIconWrapBox->AddChild(BuffWidget);
		}
	}
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

	return  OwnerUnit->CurrentULTGage / OwnerUnit->MaxULTGage;
}
