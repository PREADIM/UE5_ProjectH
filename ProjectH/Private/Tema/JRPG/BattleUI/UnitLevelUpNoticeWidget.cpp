// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/UnitLevelUpNoticeWidget.h"
#include "Components/TextBlock.h"



void UUnitLevelUpNoticeWidget::Init(FString Name, int32 Level)
{
	Notice->SetText(FText::FromString(Name));
	LevelNotice->SetText(FText::FromString(FString::FromInt(Level)));
	PlayExpAnim();
}



void UUnitLevelUpNoticeWidget::PlayExpAnim()
{
	PlayAnimation(NoticeAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);
	FTimerHandle Handle;

	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
		{
			RemoveFromParent();
		}), NoticeAnimation->GetEndTime(), false);

}
