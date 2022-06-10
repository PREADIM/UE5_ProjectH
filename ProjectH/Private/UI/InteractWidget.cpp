// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractWidget.h"
#include "Components/TextBlock.h"


void UInteractWidget::SetName(FString str)
{
	Name->SetText(FText::FromString(str));
}


void UInteractWidget::PlayAnim()
{
	PlayAnimation(InteractFade, 0.f, 1, EUMGSequencePlayMode::Forward);
}