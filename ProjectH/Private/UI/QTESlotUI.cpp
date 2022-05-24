// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QTESlotUI.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UQTESlotUI::NativeConstruct()
{
	Super::NativeConstruct();

	KeyText->SetText(Key.GetDisplayName());
	UpdateTime(0.0f);
}



void UQTESlotUI::UpdateTime(float PressTime)
{
	PressedTime = PressTime; // 호출 될때마다 시간이 증가한다.

	float Percent = 1 - (PressedTime / TotalTime);
	TimeBar->SetPercent(Percent); // 1에서 빼야 시간이 지날수록 0.0에서 1.0으로 증가한다. 
}


float UQTESlotUI::PlayWidgetAnim(bool bFlag)
{
	PlayAnimation(bFlag ? Success : Fail, 0.0f, 1, EUMGSequencePlayMode::Forward);

	return bFlag ? Success->GetEndTime() : Fail->GetEndTime();
}