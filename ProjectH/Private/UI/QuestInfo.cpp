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
#include "GameMode/ProjectHGameInstance.h"


void UQuestInfo::Init()
{
	SetText();
}


void UQuestInfo::SetText()
{
	QuestNameText->SetText(FText::FromString(NPCQuest.QuestName));
	QuestDescriptionText->SetText(FText::FromString(NPCQuest.QuestDescription));
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


void UQuestInfo::BindingFunction()
{
	Accept->OnClicked.AddDynamic(this, &UQuestInfo::AcceptClick);
	Decline->OnClicked.AddDynamic(this, &UQuestInfo::DeclineClick);
}


void UQuestInfo::AcceptClick()
{
	if (QuestComponent && OwnerController)
	{
		QuestComponent->AddQuest(NPCQuest);

		if (!GI)
			return;

		AQuestNPCBase* OwnerNPC = GI->GetNPCPtr(NPCQuest.OwnerNPCName);
		if (OwnerNPC) // 안전 검사.
		{
			OwnerNPC->QuestingNums.Emplace(NPCQuest.QuestNumber);
			OwnerNPC->SaveNPCQuest();
			OwnerController->SetQuestCollisionSetup();
		}

		QuestComponent->AcceptQuest();
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