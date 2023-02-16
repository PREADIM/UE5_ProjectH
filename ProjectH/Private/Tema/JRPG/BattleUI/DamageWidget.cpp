// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/DamageWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Tema/JRPG/JRPGPlayerController.h"

void UDamageWidget::Init(float Damage, FVector TargetLocation)
{
	if (OwnerController)
	{
		FVector2D Pos;
		DamageAmount = Damage;
		int32 IntDamage = FMath::FloorToInt(Damage);
		DamageText->SetText(FText::FromString(FString::FromInt(IntDamage)));
		OwnerController->ProjectWorldLocationToScreen(TargetLocation, Pos);
		float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld()->GetGameViewport());
		class UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DamageText->Slot);
		if (CanvasSlot)
		{
			CanvasSlot->SetPosition(FVector2D(Pos.X / Scale, Pos.Y / Scale));
		}
		AddToViewport();
		PlayAnim();
	}	
}



void UDamageWidget::PlayAnim()
{
	Super::NativeConstruct();
	PlayAnimation(DamageAnim, 0.f, 1, EUMGSequencePlayMode::Forward);
}