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
#include "Tema/JRPG/BattleUI/SkillDetailWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UJRPGBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();
}



void UJRPGBattleWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!OwnerController)
		return;

	if (LockOnIcon && OwnerController->TargetUnit)
	{	
		TargetLockOn = OwnerController->TargetUnit->GetMesh()->GetSocketLocation(FName("pelvis"));
		OwnerController->ProjectWorldLocationToScreen(TargetLockOn, Pos);
		float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld()->GetGameViewport());
		if (CanvasSlot)
			CanvasSlot->SetPosition(FVector2D(Pos.X / Scale, (Pos.Y / Scale) - 30.f));
	}

}



void UJRPGBattleWidget::Init()
{
	JRPGPriorityList->GM = GM;
	JRPGPriorityList->OwnerController = OwnerController;

	NormalAttack->GM = GM;
	NormalAttack->OwnerMainWidget = this;
	NormalAttack->OwnerController = OwnerController;
	NormalAttack->SkillButton->OnHovered.AddDynamic(this, &UJRPGBattleWidget::OnNormalAttaclDetail);
	NormalAttack->SkillButton->OnUnhovered.AddDynamic(this, &UJRPGBattleWidget::OffSkillDetail);

	SkillButton->GM = GM;
	SkillButton->OwnerMainWidget = this;
	SkillButton->OwnerController = OwnerController;
	SkillButton->SkillButton->OnHovered.AddDynamic(this, &UJRPGBattleWidget::OnSkillAttackDetail);
	SkillButton->SkillButton->OnUnhovered.AddDynamic(this, &UJRPGBattleWidget::OffSkillDetail);

	ULTButton->GM = GM;
	ULTButton->OwnerMainWidget = this;
	ULTButton->OwnerController = OwnerController;
	ULTButton->ULTButton->OnHovered.AddDynamic(this, &UJRPGBattleWidget::OnULTAttackDetail);
	ULTButton->ULTButton->OnUnhovered.AddDynamic(this, &UJRPGBattleWidget::OffSkillDetail);

	AttackDetail.Init(FAttackNameAndDetail(), 3);

	DetailWidget->SetRenderOpacity(0.f);

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


void UJRPGBattleWidget::PrioirtyBuffSetup(AJRPGUnit* Unit)
{
	JRPGPriorityList->SetupBuffIcon(Unit->CharNum);
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

		LockOnIcon->SetRenderOpacity(1.0f);
	}
	else
	{
		SkillAndListButtonHidden(true);
		LockOnIcon->SetRenderOpacity(0.0f);
	}
}


void UJRPGBattleWidget::SkillAndListButtonHidden(bool bFlag)
{
	if (bFlag)
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
	// 회전값이 바뀔때 아이콘 위치도 바뀌어야 한다.
	if (LockOnIcon)
	{
		if (!LockOnIcon->IsInViewport())
			LockOnIcon->AddToViewport();

		if (GM->EnermyList.IsValidIndex(Num))
			OwnerController->TargetUnit = GM->EnermyList[Num];
		TargetNumber = Num;
	}	
}

void UJRPGBattleWidget::VisibleLockOn()
{
	if (LockOnIcon)
		LockOnIcon->AddToViewport();
}

void UJRPGBattleWidget::HiddenLockOn()
{
	if (LockOnIcon)
		LockOnIcon->RemoveFromParent();
}


void UJRPGBattleWidget::BattleTurnInit()
{
	/* 여기서 텍스트를 가져오자. */
	FJRPGUnitAttackDetail* Detail = GM->GetAttackDetail(GM->SetUnitList[0].Unit->CharNum);

	if (Detail)
	{
		AttackDetail[0] = Detail->NormalAttackDetail;
		AttackDetail[1] = Detail->SkillAttackDetail;
		AttackDetail[3] = Detail->ULTAttackDetail;
	}
	else
	{
		AttackDetail[0] = FAttackNameAndDetail();
		AttackDetail[1] = FAttackNameAndDetail();
		AttackDetail[3] = FAttackNameAndDetail();
	}

	NormalAttack->Init();
	SkillButton->Init();
	ULTButton->Init();
}


void UJRPGBattleWidget::OnNormalAttaclDetail()
{
	DetailWidget->Init(AttackDetail[0].AttackName, AttackDetail[0].AttackDetail);
	DetailWidget->SetRenderOpacity(1.f);
}

void UJRPGBattleWidget::OnSkillAttackDetail()
{
	DetailWidget->Init(AttackDetail[1].AttackName, AttackDetail[1].AttackDetail);
	DetailWidget->SetRenderOpacity(1.f);
}


void UJRPGBattleWidget::OnULTAttackDetail()
{
	DetailWidget->Init(AttackDetail[2].AttackName, AttackDetail[2].AttackDetail);
	DetailWidget->SetRenderOpacity(1.f);
}


void UJRPGBattleWidget::OffSkillDetail()
{
	DetailWidget->SetRenderOpacity(0.f);
}