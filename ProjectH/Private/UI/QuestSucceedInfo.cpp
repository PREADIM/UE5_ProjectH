// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestSucceedInfo.h"
#include "UI/QuestList.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "Controller/ProjectH_PC.h"
#include "UI/MainQuestUI.h"
#include "AI/QuestNPCBase.h"
#include "GameMode/ProjectHGameInstance.h"



void UQuestSucceedInfo::Init()
{
	SetText();
}


void UQuestSucceedInfo::SetText()
{
	QuestNameText->SetText(FText::FromString(NPCQuest.QuestName));
	QuestDescriptionText->SetText(FText::FromString(NPCQuest.QuestDescription));
	SetColorPenSize();
}

/* ����Ʈ �̸� ����� ����ؼ� ������ �̹��� �����ϱ�. (����Ʈ ���Կ��� ����.)*/
void UQuestSucceedInfo::SetColorPenSize()
{
	// ��������Ʈ�� ��Ÿ���� ���� �Ϲ�����Ʈ ���� �ٸ��� �ϱ�.
	if (NPCQuest.QuestType == EQuestType::Main)
	{
		ColorPen->SetBrushTintColor(FSlateColor(MainColor));
		SetOutline(true);
	}
	else
	{
		ColorPen->SetBrushTintColor(FSlateColor(NormalColor));
		SetOutline(false);
	}

	UCanvasPanelSlot* CanvasSlot = Canvas->AddChildToCanvas(ColorPen);
	if (CanvasSlot)
	{
		CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.f));
		CanvasSlot->SetPosition(FVector2D(0.0f, -215.f));
		int32 len = NPCQuest.QuestName.Len();
		if (len < 17)
		{
			CanvasSlot->SetSize(FVector2D(len * 50, 50.f));
		}
		else
		{
			CanvasSlot->SetSize(FVector2D(850.f, 50.f));
		}
		CanvasSlot->SetZOrder(0);
	}
}


void UQuestSucceedInfo::BindingFunction()
{
	Succeed->OnClicked.AddDynamic(this, &UQuestSucceedInfo::SucceedClick);
	Decline->OnClicked.AddDynamic(this, &UQuestSucceedInfo::DeclineClick);
}


void UQuestSucceedInfo::SucceedClick()
{
	if (QuestComponent && OwnerController)
	{
		if (!GI)
			return;

		AQuestNPCBase* OwnerNPC = GI->GetNPCPtr(NPCQuest.OwnerNPCName);
		if (OwnerNPC) // ���� �˻�.
		{
			OwnerNPC->EndedQuestsNums.Emplace(NPCQuest.QuestNumber);
			OwnerNPC->SaveNPCQuest();
			QuestComponent->RemoveQuest(NPCQuest.QuestNumber);
			OwnerController->SetQuestCollisionSetup();
		}

		DeclineClick();
	}

}


void UQuestSucceedInfo::DeclineClick()
{
	if (OwnerController)
	{
		OwnerController->MainQuestUI->QuestSucceedInfoAnimation(true); // ����
	}
}