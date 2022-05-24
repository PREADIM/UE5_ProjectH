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
	PressedTime = PressTime; // ȣ�� �ɶ����� �ð��� �����Ѵ�.

	float Percent = 1 - (PressedTime / TotalTime);
	TimeBar->SetPercent(Percent); // 1���� ���� �ð��� �������� 0.0���� 1.0���� �����Ѵ�. 
}


float UQTESlotUI::PlayWidgetAnim(bool bFlag)
{
	PlayAnimation(bFlag ? Success : Fail, 0.0f, 1, EUMGSequencePlayMode::Forward);

	return bFlag ? Success->GetEndTime() : Fail->GetEndTime();
}