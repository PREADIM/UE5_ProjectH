// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QTEMainUI.h"
#include "UI/QTESlotUI.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UQTESlotUI* UQTEMainUI::AddQTEWidget(FKey Key, float TotalTime, FQTEWidgetPosition ScreenPosition)
{
	// ��÷���� ������Ѵ�.�̹� �ִµ��� �ұ��ϰ� �� CreateWidget�� �Ͽ� AddChildToCanvas �� ���� �����Ƿ� ������ �˻��Ѵ�.
	if (!QTEWidget) 
	{
		if (BP_QTEWidget)
			QTEWidget = CreateWidget<UQTESlotUI>(GetWorld(), BP_QTEWidget);

		if (QTEWidget)
		{
			QTEWidget->Key = Key;
			QTEWidget->TotalTime = TotalTime;

			UCanvasPanelSlot* CanvasSlot = Canvas->AddChildToCanvas(QTEWidget);

			if (CanvasSlot)
			{
				CanvasSlot->SetAutoSize(true);
				CanvasSlot->SetAnchors(ScreenPosition.Anchor);
				CanvasSlot->SetAlignment(ScreenPosition.Alignment);
				CanvasSlot->SetPosition(ScreenPosition.Position);
			}
		}
	}

	return QTEWidget;
}