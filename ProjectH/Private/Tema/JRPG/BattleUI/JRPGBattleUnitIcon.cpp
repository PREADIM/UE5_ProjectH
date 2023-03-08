// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGBattleUnitIcon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Wrapbox.h"
#include "Tema/JRPG/BattleUI/JRPGBuffWidget.h"


void UJRPGBattleUnitIcon::SetDebuffIcon(TSet<FDebuffStruct> DebuffSet)
{
	if (!BP_BuffWidget)
		return;
		
	BuffWidgets.Empty();
	BuffIconWrapBox->ClearChildren();
	for (FDebuffStruct DebuffStruct : DebuffSet)
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

void UJRPGBattleUnitIcon::Init(UTexture2D* CharTex, FString CharName)
{
	CharImg->SetBrushFromTexture(CharTex);
	CharText->SetText(FText::FromString(CharName));
	PlayInitAnim();
}

void UJRPGBattleUnitIcon::PlayInitAnim()
{
	PlayAnimation(InitIcon, 0.0f, 1, EUMGSequencePlayMode::Forward);
}

void UJRPGBattleUnitIcon::PlayCurrentAnim()
{
	PlayAnimation(CurrentUnit, 0.0f, 1, EUMGSequencePlayMode::Forward);
}

