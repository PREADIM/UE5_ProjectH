// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/CAWTextAnimWidget.h"
#include "Components/TextBlock.h"

void UCAWTextAnimWidget::SetCAWText(FString Text)
{
	CAWText->SetText(FText::FromString(Text));
}