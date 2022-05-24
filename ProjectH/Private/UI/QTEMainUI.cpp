// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QTEMainUI.h"
#include "UI/QTESlotUI.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UQTESlotUI* UQTEMainUI::AddQTEWidget(FKey Key, float TotalTime, FQTEWidgetPosition ScreenPosition)
{
	// 맨첨에는 없어야한다.이미 있는데도 불구하고 또 CreateWidget을 하여 AddChildToCanvas 할 수도 있으므로 없는지 검사한다.
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