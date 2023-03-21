// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestMainCanvasWidget.h"
#include "UI/NormalIconUI.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PlayerControllerBase.h"


UCanvasPanelSlot* UQuestMainCanvasWidget::AddChildCanvas(UUserWidget* Widget)
{
	return Canvas->AddChildToCanvas(Widget);
}


void UQuestMainCanvasWidget::MainQuestIconWidgetSetup(UCanvasPanelSlot* CanvasSlot, FVector Location)
{
	if (!OwnerController || !CanvasSlot)
		return;

	FVector2D Pos;
	OwnerController->ProjectWorldLocationToScreen(Location, Pos);

	float VScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	float ViewportSizeX = (UWidgetLayoutLibrary::GetViewportSize(GetWorld()).X / VScale) - 100.f;
	float ViewportSizeY = (UWidgetLayoutLibrary::GetViewportSize(GetWorld()).Y / VScale) - 100.f;

	float X = FMath::Clamp(Pos.X / VScale, 100.f, ViewportSizeX);
	float Y = FMath::Clamp(Pos.Y / VScale, 100.f, ViewportSizeY);

	CanvasSlot->SetPosition(FVector2D(X, Y));
}