// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/DropExpWidget.h"
#include "Components/TextBlock.h"



void UDropExpWidget::Init(float Exp)
{
	DropExp->SetText(FText::FromString(FString::FromInt(Exp)));
	PlayExpAnim();
}



void UDropExpWidget::PlayExpAnim()
{
	PlayAnimation(ExpAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);
	FTimerHandle Handle;

	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
		{
			RemoveFromParent();
		}), ExpAnimation->GetEndTime(), false);

}
