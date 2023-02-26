// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ActiveQuestSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/QuestList.h"



/*---------------
	public Func
----------------*/

void UActiveQuestSlot::Init()
{
	QuestNameText->SetText(FText::FromString(QuestName));
	if (QuestComponent)
	{
		BindQuestDescription(); // 퀘스트 목표 설정
		ActiveQuestColor(); // 현재 활성화 퀘스트 색 변경
		QuestClearButton->OnClicked.AddDynamic(this, &UActiveQuestSlot::ClearButtonClicked);
	}
}


void UActiveQuestSlot::BindQuestDescription()
{
	QuestDescriptionText->SetText(FText::FromString(QuestDescription));
}


void UActiveQuestSlot::ActiveQuestColor()
{
	//FSlateColor를 사용하려면 Build.cs 파일에 "SlateCore"를 추가해야한다.
	SetOutline(true);
	SetColorPenSize();
	QuestNameText->SetColorAndOpacity(FSlateColor(SelectColor));
}

void UActiveQuestSlot::ClearButtonClicked()
{
	if (OwnerList)
	{
		OwnerList->ActiveClearButtonClick();
	}

}

/* ★ 글자 크기에 따라서 이미지 크기 조정 하기. ★ */
void UActiveQuestSlot::SetColorPenSize()
{
	UCanvasPanelSlot* CanvasSlot = Canvas->AddChildToCanvas(ColorPen);

	if (CanvasSlot)
	{
		CanvasSlot->SetPosition(FVector2D(0.f, 15.f));
		int32 len = QuestName.Len();
		if (len < 14)
		{
			CanvasSlot->SetSize(FVector2D(len * 40, 50.f));
		}
		else
		{
			CanvasSlot->SetSize(FVector2D(550.f, 50.f));
		}
		CanvasSlot->SetZOrder(-1);
	}
	SetColorPen();
}

/* 형광펜 강조를 진행중인 퀘스트만 보이게한다. */
void UActiveQuestSlot::SetColorPen()
{
	ColorPen->SetRenderOpacity(1.0f);
}


