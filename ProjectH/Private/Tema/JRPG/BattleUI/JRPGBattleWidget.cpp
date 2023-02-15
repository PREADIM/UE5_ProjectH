// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGBattleWidget.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/BattleUI/JRPGPriority.h"
#include "Tema/JRPG/BattleUI/JRPGSkillButton.h"
#include "Tema/JRPG/BattleUI/JRPGULTButton.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Tema/JRPG/BattleUI/EnermyIconButton.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UJRPGBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TargetNumber = 0;
}



void UJRPGBattleWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (TargetUnit)
	{	
		TargetLockOn = TargetUnit->GetActorLocation();
		OwnerController->ProjectWorldLocationToScreen(TargetLockOn, Pos);
		float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld()->GetGameViewport());

		//FVector2D(Pos.X / Scale, Pos.Y / Scale)
		//LockOnIcon->SetPositionInViewport(Pos);
		//_DEBUG("%f, %f", Pos.X, Pos.Y);
	}

}



void UJRPGBattleWidget::Init()
{
	JRPGPriorityList->GM = GM;
	JRPGPriorityList->OwnerController = OwnerController;

	NormalAttack->GM = GM;
	NormalAttack->OwnerController = OwnerController;
	SkillButton->GM = GM;
	SkillButton->OwnerController = OwnerController;
	ULTButton->GM = GM;
	ULTButton->OwnerController = OwnerController;

	if (BP_LockOnIcon)
	{
		LockOnIcon = CreateWidget<UUserWidget>(GetWorld(), BP_LockOnIcon);
	}
}


void UJRPGBattleWidget::PlayPriority()
{
	JRPGPriorityList->PlayInit();
}

void UJRPGBattleWidget::SetUnitList()
{
	JRPGPriorityList->SetUnitList();
}

void UJRPGBattleWidget::EnermyListBeginInit()
{
	EnermyListInit();
	// 맨처음 실행하는 것이라서, 여기에 애니메이션 실행하는 것도 ㄱㅊ을듯.
}

void UJRPGBattleWidget::EnermyListInit()
{
	if (BP_EnermyIcon && GM)
	{
		EnermyList->ClearChildren();
		TArray<AJRPGUnit*> Units = GM->EnermyList;
		for (int32 i = 0; i < Units.Num(); i++)
		{
			UEnermyIconButton* Button = CreateWidget<UEnermyIconButton>(GetWorld(), BP_EnermyIcon);
			FJRPGUnitUIStruct* UI = OwnerController->GetUnitUI(Units[i]->CharNum);

			if (Button && UI)
			{
				Button->GM = GM;
				Button->OwnerController = OwnerController;
				Button->OwnerWidget = this;
				Button->Init(UI->CharTex, i);
				Button->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));
				EnermyList->AddChild(Button);
				Buttons.Add(Button);
			}
		}
		Buttons[TargetNumber]->TargetLockOn();
	}
}

void UJRPGBattleWidget::TargetToRotation()
{
	Buttons[TargetNumber]->TargetLockOn();
}

void UJRPGBattleWidget::EnermyTargetToRotation()
{
	AJRPGUnit* SelfUnit = GM->SetUnitList[0].Unit;

	if (OwnerController->TargetUnit)
	{
		FVector TargetLocation = OwnerController->TargetUnit->GetActorLocation();
		FVector SelfLocation = SelfUnit->GetActorLocation();
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(TargetLocation, SelfLocation);
		FRotator EnermyRot = UKismetMathLibrary::FindLookAtRotation(SelfLocation, TargetLocation);

		OwnerController->CameraSetUp(TargetLocation);
		OwnerController->CameraRotSetUp(Rot);

		SelfUnit->SetActorRotation(EnermyRot);
	}
	else
	{
		SelfUnit->SetActorRelativeRotation(FRotator(0.f, 0.f, 90.f));
	}

}


void UJRPGBattleWidget::SetVisible(bool bFlag)
{
	if (bFlag)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}


void UJRPGBattleWidget::EnermyTurnHidden(bool bFlag)
{
	if (bFlag) // 적 차례시
	{
		NormalAttack->SetVisibility(ESlateVisibility::Hidden);
		SkillButton->SetVisibility(ESlateVisibility::Hidden);
		ULTButton->SetVisibility(ESlateVisibility::Hidden);
		EnermyList->SetVisibility(ESlateVisibility::Hidden);
		LockOnIcon->SetVisibility(ESlateVisibility::Hidden);

	}
	else
	{
		NormalAttack->SetVisibility(ESlateVisibility::Visible);
		SkillButton->SetVisibility(ESlateVisibility::Visible);
		ULTButton->SetVisibility(ESlateVisibility::Visible);
		EnermyList->SetVisibility(ESlateVisibility::Visible);
		LockOnIcon->SetVisibility(ESlateVisibility::Visible);
	}
}

void UJRPGBattleWidget::SetButtonVisible(bool bFlag)
{
	if (bFlag)
	{
		NormalAttack->SetVisibility(ESlateVisibility::Visible);
		SkillButton->SetVisibility(ESlateVisibility::Visible);
		ULTButton->SetVisibility(ESlateVisibility::Visible);
		SetLockOn(TargetNumber);
		bButtonVisible = true;
	}
	else
	{
		NormalAttack->SetVisibility(ESlateVisibility::Hidden);
		SkillButton->SetVisibility(ESlateVisibility::Hidden);
		ULTButton->SetVisibility(ESlateVisibility::Hidden);
		HiddenLockOn();
		bButtonVisible = false;
	}
}

void UJRPGBattleWidget::SetLockOn(int32 Num)
{
	// 회전값이 바뀔때 아이콘 위치도 바뀌어야 한다.
	if (LockOnIcon)
	{
		if (!LockOnIcon->IsInViewport())
		{
			LockOnIcon->AddToViewport();
		}

		if (GM->EnermyList.IsValidIndex(Num))
		{
			TargetUnit = GM->EnermyList[Num];
			OwnerController->TargetUnit = TargetUnit;
		}

		TargetNumber = Num;
	}
	
}

void UJRPGBattleWidget::HiddenLockOn()
{
	if (LockOnIcon)
	{
		LockOnIcon->RemoveFromParent();
	}
}


void UJRPGBattleWidget::BattleTurnInit()
{
	NormalAttack->Init();
	SkillButton->Init();
	ULTButton->Init();
}

void UJRPGBattleWidget::EnermyTurnFirst()
{
	NormalAttack->EnermyTurnFirstInit();
	SkillButton->EnermyTurnFirstInit();
	ULTButton->EnermyTurnFirstInit();
}

