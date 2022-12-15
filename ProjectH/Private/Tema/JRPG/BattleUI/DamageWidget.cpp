// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/DamageWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Tema/JRPG/JRPGPlayerController.h"

void UDamageWidget::Init(float Damage, FVector TargetLocation)
{
	if (OwnerController)
	{
		FVector2D Pos;
		DamageAmount = Damage;
		//★//★ DamageText->SetText(UKismetTextLibrary::Conv_FloatToText(DamageAmount, ERoundingMode::HalfToEven, false, true, 1, 324, 0, 0));
		DamageText->SetText(FText::FromString(FString::SanitizeFloat(Damage)));
		// ★ 나중에 5.1 버그 고치고 테스트.
		OwnerController->ProjectWorldLocationToScreen(TargetLocation, Pos);
		SetPositionInViewport(Pos);
		AddToViewport();
		PlayAnim();
	}	
}



void UDamageWidget::PlayAnim()
{
	Super::NativeConstruct();
	PlayAnimation(DamageAnim, 0.f, 1, EUMGSequencePlayMode::Forward);
}