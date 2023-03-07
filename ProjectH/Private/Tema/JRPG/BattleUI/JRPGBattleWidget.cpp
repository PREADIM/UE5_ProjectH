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
#include "Tema/JRPG/BattleUI/LockOnWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Tema/JRPG/BattleUI/LockOnWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UJRPGBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//TargetNumber = 0;
}



void UJRPGBattleWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (LockOnIcon && TargetUnit)
	{	
		TargetLockOn = TargetUnit->GetMesh()->GetSocketLocation(FName("pelvis"));
		OwnerController->ProjectWorldLocationToScreen(TargetLockOn, Pos);
		float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld()->GetGameViewport());
		if (CanvasSlot)
		{
			CanvasSlot->SetPosition(FVector2D(Pos.X / Scale, (Pos.Y / Scale) - 30.f));
		}
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
		LockOnIcon = CreateWidget<ULockOnWidget>(GetWorld(), BP_LockOnIcon);
		if(LockOnIcon)
			CanvasSlot = Cast<UCanvasPanelSlot>(LockOnIcon->Image->Slot);
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
		if (!LockOnIcon)
		{
			LockOnIcon = CreateWidget<ULockOnWidget>(GetWorld(), BP_LockOnIcon);
			LockOnIcon->AddToViewport();
		}

		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		LockOnIcon->SetRenderOpacity(1.0f);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		LockOnIcon->SetRenderOpacity(0.0f);
	}
}


void UJRPGBattleWidget::EnermyTurnHidden(bool bFlag)
{
	if (bFlag) // �� ���ʽ�
	{
		NormalAttack->SetVisibility(ESlateVisibility::Hidden);
		SkillButton->SetVisibility(ESlateVisibility::Hidden);
		ULTButton->SetVisibility(ESlateVisibility::Hidden);
		EnermyList->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		NormalAttack->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SkillButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ULTButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		EnermyList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}


void UJRPGBattleWidget::SetLockOn(int32 Num)
{
	// ȸ������ �ٲ� ������ ��ġ�� �ٲ��� �Ѵ�.
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
		LockOnIcon->RemoveFromParent();
}


void UJRPGBattleWidget::BattleTurnInit()
{
	NormalAttack->Init();
	SkillButton->Init();
	ULTButton->Init();
}

//void UJRPGBattleWidget::EnermyTurnFirst()
//{
//	NormalAttack->EnermyTurnFirstInit();
//	SkillButton->EnermyTurnFirstInit();
//	ULTButton->EnermyTurnFirstInit();
//}

