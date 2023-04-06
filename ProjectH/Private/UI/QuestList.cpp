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


// ����Ʈâ �ʱ�ȭ. ��Ƽ�� ����Ʈ�� �׻� �ֻ�ܿ� �߰��Ѵ�.
void UQuestList::UpdateQuestList()
{
	AQSlot = nullptr;
	ActiveQuestIndex = -1;
	SlotArr.Empty(); // ����Ʈ ���� �ʱ�ȭ.
	CurrentQuest->ClearChildren(); // ������ �ٽ� �����ϹǷ� ���⼭ �ʱ�ȭ.
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
		// ���� ��Ƽ������Ʈ ������ ���� ����Ʈ���,
		if (QuestComponent->GetActiveQuest().QuestName == SlotArr[QuestIndex]->QuestName)
		{
			AQSlot->QuestDescription = QuestComponent->GetQuests()[QuestIndex].QuestSteps[0].Description;
			AQSlot->BindQuestDescription();
		}

	}

	SlotArr[QuestIndex]->QuestDescription = QuestComponent->GetQuests()[QuestIndex].QuestSteps[0].Description;
	SlotArr[QuestIndex]->BindQuestDescription(); // ������ ����ϸ�ȴ�.
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
	// ������ �̹���� ���ɸ���. �ͱ�?  

	QuestComponent->GetQuests()[ActiveQuestIndex].QuestSteps[0].Trigger->SetHiddenTriggerWidget(); // ���� �����ִ� Ʈ������ ƽ�� ���� ��Ȱ��ȭ.
	QuestComponent->ActiveQuestClear();
	UpdateQuestList(); // ��ü ������ ���Ƽ� ������ �̰� �� ������ ����� �� �˾�����, ������ �̰� ���� �Ȱɸ���.
}