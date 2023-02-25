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
		BindQuestDescription(); // ����Ʈ ��ǥ ����
		ActiveQuestColor(); // ���� Ȱ��ȭ ����Ʈ �� ����
		QuestFollow->OnClicked.AddDynamic(this, &UQuestSlot::SelectButtonClicked);
	}
}


void UQuestSlot::BindQuestDescription()
{
	QuestDescriptionText->SetText(FText::FromString(QuestDescription));
	// QuestCashes�� ����Ʈ �̸����� �˻��ؼ� �� ����Ʈ�� �Ϸ� ������ ���.
}


void UQuestSlot::ActiveQuestColor()
{

	//FSlateColor�� ����Ϸ��� Build.cs ���Ͽ� "SlateCore"�� �߰��ؾ��Ѵ�.
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

/* �� ���� ũ�⿡ ���� �̹��� ũ�� ���� �ϱ�. �� */
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

/* ������ ������ �������� ����Ʈ�� ���̰��Ѵ�. */
void UQuestSlot::SetColorPen()
{
	ColorPen->SetRenderOpacity(1.0f);
}

