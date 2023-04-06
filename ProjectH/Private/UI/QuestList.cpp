// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestList.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "UI/QuestSlot.h"
#include "UI/ActiveQuestSlot.h"
#include "QuestStruct.h"
#include "ActorComponent/QuestComponent/TriggerEventBase.h"


void UQuestList::Init()
{
	if (QuestComponent)
	{
		QuestComponent->OnUpdateQuestList.AddUFunction(this, FName("UpdateQuestList"));
		QuestComponent->OnUpdateDescription.AddUFunction(this, FName("CompleteStepSetSlot"));
	}
}


// 퀘스트창 초기화. 액티브 퀘스트는 항상 최상단에 뜨게한다.
void UQuestList::UpdateQuestList()
{
	AQSlot = nullptr;
	ActiveQuestIndex = -1;
	SlotArr.Empty(); // 퀘스트 슬롯 초기화.
	CurrentQuest->ClearChildren(); // 슬롯을 다시 생성하므로 여기서 초기화.
	MainQuest->ClearChildren();
	OtherQuests->ClearChildren();


	for (int32 i = 0; i < QuestComponent->GetQuests().Num(); ++i)
	{
		if (QuestComponent->GetQuests()[i].QuestNumber == QuestComponent->GetActiveQuest().QuestNumber)
		{
			AQSlot = CreateWidget<UActiveQuestSlot>(GetWorld(), BP_ActiveQuestSlot);
			if (AQSlot)
			{
				AQSlot->QuestName = QuestComponent->GetQuests()[i].QuestName;
				AQSlot->QuestDescription = QuestComponent->GetQuests()[i].QuestSteps[0].Description;
				AQSlot->QuestComponent = QuestComponent;
				AQSlot->OwnerList = this;
				ActiveQuestIndex = i;
				AQSlot->Init();
				AQSlot->SetPadding(FMargin(120.f, 0.f, 0.f, 15.f));
				CurrentQuest->AddChild(AQSlot);
			}
		}

		UQuestSlot* QSlot = CreateWidget<UQuestSlot>(GetWorld(), BP_QuestSlot);
		if (QSlot)
		{
			QSlot->QuestName = QuestComponent->GetQuests()[i].QuestName;
			QSlot->QuestDescription = QuestComponent->GetQuests()[i].QuestSteps[0].Description;
			QSlot->QuestComponent = QuestComponent;
			QSlot->Init();
			QSlot->SetPadding(FMargin(120.f, 0.f, 0.f, 80.f));
			switch (QuestComponent->GetQuests()[i].QuestType)
			{
			case EQuestType::Main:
				MainQuest->AddChild(QSlot);
				break;
			case EQuestType::Normal:
				OtherQuests->AddChild(QSlot);
				break;
			}
			SlotArr.Emplace(QSlot);
		}
	}
}



void UQuestList::CompleteStepSetSlot(int32 QuestIndex)
{
	if (!SlotArr.IsValidIndex(QuestIndex))
	{
		return;
	}

	if (AQSlot)
	{
		// 현재 액티브퀘스트 슬롯이 같은 퀘스트라면,
		if (QuestComponent->GetActiveQuest().QuestName == SlotArr[QuestIndex]->QuestName)
		{
			AQSlot->QuestDescription = QuestComponent->GetQuests()[QuestIndex].QuestSteps[0].Description;
			AQSlot->BindQuestDescription();
		}

	}

	SlotArr[QuestIndex]->QuestDescription = QuestComponent->GetQuests()[QuestIndex].QuestSteps[0].Description;
	SlotArr[QuestIndex]->BindQuestDescription(); // 설명서만 출력하면된다.
}



void UQuestList::ActiveClearButtonClick()
{
	/*if (SlotArr.IsValidIndex(ActiveQuestIndex))
	{
		QuestComponent->GetQuests()[ActiveQuestIndex].QuestSteps[0].Trigger->SetHiddenTriggerWidget();
		QuestComponent->GetActiveQuest().Clear();
		CurrentQuest->ClearChildren();
		SlotArr[ActiveQuestIndex]->Init();	
	} */
	// 오히려 이방식이 렉걸린다. 왤까?  

	QuestComponent->GetQuests()[ActiveQuestIndex].QuestSteps[0].Trigger->SetHiddenTriggerWidget(); // 먼저 열려있는 트리거의 틱과 위젯 비활성화.
	QuestComponent->ActiveQuestClear();
	UpdateQuestList(); // 전체 포문을 돌아서 오히려 이게 더 안좋은 방식인 줄 알았으나, 오히려 이게 렉이 안걸린다.
}