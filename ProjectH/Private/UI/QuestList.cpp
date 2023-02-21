// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestList.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "UI/QuestSlot.h"
#include "QuestStruct.h"


void UQuestList::Init()
{
	if (QuestComponent)
	{
		QuestComponent->OnAddedRemovedQuest.AddUFunction(this, FName("UpdateQuestList"));
		QuestComponent->ActiveQuestChange.AddUFunction(this, FName("UpdateQuestList"));
		QuestComponent->CompleteQuestDelegate.AddUFunction(this, FName("CompleteStepSetSlot"));
		QuestComponent->SetDescriptionDelegate.AddUFunction(this, FName("CompleteStepSetSlot"));
	}
}


// 퀘스트창 초기화. 액티브 퀘스트는 항상 최상단에 뜨게한다.
void UQuestList::UpdateQuestList()
{
	SlotArr.Empty(); // 퀘스트 슬롯 초기화.
	CurrentQuest->ClearChildren(); // 슬롯을 다시 생성하므로 여기서 초기화.
	MainQuest->ClearChildren();
	OtherQuests->ClearChildren();

	if (QuestComponent->GetActiveQuest().QuestName != FString("")) // 액티브 퀘스트가 있다면,
	{
		//선택한 퀘스트 최 상단에 뜨게하기.
		UQuestSlot* QSlot = CreateWidget<UQuestSlot>(GetWorld(), BP_QuestSlot);
		if (QSlot)
		{
			QSlot->QuestName = QuestComponent->GetActiveQuest().QuestName;
			QSlot->QuestComponent = QuestComponent;
			QSlot->Init();
			QSlot->SetPadding(FMargin(120.f, 0.f, 0.f, 120.f));
			CurrentQuest->AddChild(QSlot);
			SlotArr.Emplace(QSlot);
		}
		
		// 액티브 퀘스트 먼저 넣기.
	}


	for (FQuestStruct Quest : QuestComponent->GetQuests())
	{
		if (Quest.QuestName == QuestComponent->GetActiveQuest().QuestName)
			continue;
		// 액티브 퀘스트는 위에서 추가했으므로 여기서 건너뛰기.

		UQuestSlot* QSlot = CreateWidget<UQuestSlot>(GetWorld(), BP_QuestSlot);
		if (QSlot)
		{
			QSlot->QuestName = Quest.QuestName;
			QSlot->QuestComponent = QuestComponent;
			QSlot->Init();
			QSlot->SetPadding(FMargin(120.f, 0.f, 0.f, 120.f));
			switch (Quest.QuestType)
			{
			case EQuestType::Main :
				MainQuest->AddChild(QSlot);
				break;
			case EQuestType::Normal :
				OtherQuests->AddChild(QSlot);
				break;
			}
			SlotArr.Emplace(QSlot);
		}
	}
}



void UQuestList::CompleteStepSetSlot()
{
	if (!SlotArr.Num()) // 0번째가 액티브 퀘스트인데 없으면 할 필요가없다.
		return;

	SlotArr[0]->BindQuestDescription(); // 설명서만 출력하면된다.
}
