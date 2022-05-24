// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestInfo.h"
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



void UQuestInfo::Init()
{
	SetText();
}


void UQuestInfo::SetText()
{
	QuestNameText->SetText(FText::FromString(QuestName));
	QuestDescriptionText->SetText(FText::FromString(QuestDescription));
	SetColorPenSize();
}

/* ����Ʈ �̸� ����� ����ؼ� ������ �̹��� �����ϱ�. (����Ʈ ���Կ��� ����.)*/
void UQuestInfo::SetColorPenSize()
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
		int32 len = QuestName.Len();
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


void UQuestInfo::BindingFunction()
{
	Accept->OnClicked.AddDynamic(this, &UQuestInfo::AcceptClick);
	Decline->OnClicked.AddDynamic(this, &UQuestInfo::DeclineClick);
}


void UQuestInfo::AcceptClick()
{
	if (QuestComponent)
	{
		QuestComponent->AddQuest(NPCQuest);

		
		AQuestNPCBase* OwnerNPC = NPCQuest.OwnerNPC;
		if (OwnerNPC) // ���� �˻�.
		{
			if (NPCQuest.QuestType == EQuestType::Main)
			{
				OwnerNPC->CanMainQuestCnt--;
				if (OwnerNPC->CanMainQuestCnt < 1)
				{
					OwnerNPC->bHaveMainQuest = false;
				}
			} // ���� ����Ʈ�� �����ߴ��� �Ǵ��ϱ�.

			OwnerNPC->NPCQuests.Quests.RemoveAt(QuestIndex); // NPC ����Ʈ ����.
			OwnerNPC->CanQuestCnt--;
			if (OwnerNPC->CanQuestCnt < 1) // ���������� ����Ʈ�� ���̻� ����.
			{
				OwnerNPC->bCanAccept = false;
				OwnerNPC->SetActorTickEnabled(false);
				OwnerNPC->HiddenIcon();
			}
			

			//NPCQuest.OwnerNPC->bIsQuesting = true; ����Ʈ ������ �Ǵ�.
			NPCQuest.OwnerNPC->SaveNPCQuest();
		}

		QuestComponent->SaveQuestNumber(NPCQuest.QuestNumber); // ���� ������ ����Ʈ �ѹ� ����.
		QuestComponent->AcceptEvent.Broadcast();
		DeclineClick();
	}
	
}


void UQuestInfo::DeclineClick()
{
	if (OwnerController)
	{
		OwnerController->MainQuestUI->QuestInfoAnimation(true); // ����
	}
}