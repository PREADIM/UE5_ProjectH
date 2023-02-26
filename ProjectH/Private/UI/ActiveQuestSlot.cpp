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
		BindQuestDescription(); // ����Ʈ ��ǥ ����
		ActiveQuestColor(); // ���� Ȱ��ȭ ����Ʈ �� ����
		QuestClearButton->OnClicked.AddDynamic(this, &UActiveQuestSlot::ClearButtonClicked);
	}
}


void UActiveQuestSlot::BindQuestDescription()
{
	QuestDescriptionText->SetText(FText::FromString(QuestDescription));
}


void UActiveQuestSlot::ActiveQuestColor()
{
	//FSlateColor�� ����Ϸ��� Build.cs ���Ͽ� "SlateCore"�� �߰��ؾ��Ѵ�.
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

/* �� ���� ũ�⿡ ���� �̹��� ũ�� ���� �ϱ�. �� */
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

/* ������ ������ �������� ����Ʈ�� ���̰��Ѵ�. */
void UActiveQuestSlot::SetColorPen()
{
	ColorPen->SetRenderOpacity(1.0f);
}


