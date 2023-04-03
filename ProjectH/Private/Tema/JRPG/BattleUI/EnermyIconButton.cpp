// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/EnermyIconButton.h"
#include "Tema/JRPG/BattleUI/JRPGBattleWidget.h"
#include "Components/Button.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Kismet/KismetMathLibrary.h"

void UEnermyIconButton::Init(class UTexture2D* CharTex, int32 cnt)
{
	FSlateBrush Icon;
	FButtonStyle Style;
	Icon = UWidgetBlueprintLibrary::MakeBrushFromTexture(CharTex);

	Style.SetNormal(Icon);
	Style.Normal.TintColor = FSlateColor(NormalColor);

	Style.SetHovered(Icon);
	Style.Hovered.TintColor = FSlateColor(HoveredColor);

	Style.SetPressed(Icon);
	Style.Pressed.TintColor = FSlateColor(PressedColor);
	EnermyButton->WidgetStyle = Style;
	EnermyButton->OnClicked.AddDynamic(this, &UEnermyIconButton::TargetLockOn);

	Num = cnt; // ���° ĭ�� �ִ� ������.
	_DEBUG("init : %d", Num);
}

void UEnermyIconButton::TargetLockOn()
{
	if (GM && OwnerController)
	{
		if (!GM->EnermyList.IsValidIndex(Num))
		{
			if (!GM->EnermyList.IsValidIndex(0))
			{
				OwnerWidget->SetLockOn(0);
				OwnerController->OwnerUnitTurnToTarget();
				return;
			}
		}

		_DEBUG("%d", Num);
		OwnerWidget->SetLockOn(Num); /* ���⼭ TargetUnit�� �����԰� ���ÿ� LockOn ���� */
		OwnerController->OwnerUnitTurnToTarget();		
	}
}