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

/* 퀘스트 이름 사이즈에 비례해서 형광펜 이미지 조절하기. (퀘스트 슬롯에도 있음.)*/
void UQuestInfo::SetColorPenSize()
{
	// 메인퀘스트를 나타내는 색과 일반퀘스트 색을 다르게 하기.
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
		if (OwnerNPC) // 안전 검사.
		{
			if (NPCQuest.QuestType == EQuestType::Main)
			{
				OwnerNPC->CanMainQuestCnt--;
				if (OwnerNPC->CanMainQuestCnt < 1)
				{
					OwnerNPC->bHaveMainQuest = false;
				}
			} // 메인 퀘스트를 수락했는지 판단하기.

			OwnerNPC->NPCQuests.Quests.RemoveAt(QuestIndex); // NPC 퀘스트 삭제.
			OwnerNPC->CanQuestCnt--;
			if (OwnerNPC->CanQuestCnt < 1) // 수락가능한 퀘스트가 더이상 없다.
			{
				OwnerNPC->bCanAccept = false;
				OwnerNPC->SetActorTickEnabled(false);
				OwnerNPC->HiddenIcon();
			}
			

			//NPCQuest.OwnerNPC->bIsQuesting = true; 퀘스트 중인지 판단.
			NPCQuest.OwnerNPC->SaveNPCQuest();
		}

		QuestComponent->SaveQuestNumber(NPCQuest.QuestNumber); // 진행 가능한 퀘스트 넘버 수정.
		QuestComponent->AcceptEvent.Broadcast();
		DeclineClick();
	}
	
}


void UQuestInfo::DeclineClick()
{
	if (OwnerController)
	{
		OwnerController->MainQuestUI->QuestInfoAnimation(true); // 끄기
	}
}