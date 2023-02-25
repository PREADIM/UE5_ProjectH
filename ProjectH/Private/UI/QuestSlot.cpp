// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "Kismet/KismetMathLibrary.h"



/*---------------
	public Func
----------------*/

void UQuestSlot::Init()
{
	QuestNameText->SetText(FText::FromString(QuestName));
	if (QuestComponent)
	{
		BindQuestDescription(); // 퀘스트 목표 설정
		ActiveQuestColor(); // 현재 활성화 퀘스트 색 변경
		QuestFollow->OnClicked.AddDynamic(this, &UQuestSlot::SelectButtonClicked);
	}
}


void UQuestSlot::BindQuestDescription()
{
	QuestDescriptionText->SetText(FText::FromString(QuestDescription));
	// QuestCashes를 퀘스트 이름으로 검색해서 그 퀘스트의 완료 조건을 출력.
}


void UQuestSlot::ActiveQuestColor()
{

	//FSlateColor를 사용하려면 Build.cs 파일에 "SlateCore"를 추가해야한다.
	FLinearColor NameColor;
	if (QuestComponent->GetActiveQuest().QuestName == QuestName)
	{
		NameColor = SelectColor;
		SetOutline(true);
		SetColorPenSize();
	}
	else
	{
		NameColor = NormalColor;
		SetOutline(false);
	}
	QuestNameText->SetColorAndOpacity(FSlateColor(NameColor));
}

void UQuestSlot::SelectButtonClicked()
{
	QuestComponent->SelectQuest(QuestName);
}

/* ★ 글자 크기에 따라서 이미지 크기 조정 하기. ★ */
void UQuestSlot::SetColorPenSize()
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
void UQuestSlot::SetColorPen()
{
	ColorPen->SetRenderOpacity(1.0f);
}

