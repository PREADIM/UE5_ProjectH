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


// ����Ʈâ �ʱ�ȭ. ��Ƽ�� ����Ʈ�� �׻� �ֻ�ܿ� �߰��Ѵ�.
void UQuestList::UpdateQuestList()
{
	SlotArr.Empty(); // ����Ʈ ���� �ʱ�ȭ.
	CurrentQuest->ClearChildren(); // ������ �ٽ� �����ϹǷ� ���⼭ �ʱ�ȭ.
	MainQuest->ClearChildren();
	OtherQuests->ClearChildren();

	if (QuestComponent->GetActiveQuest().QuestName != FString("")) // ��Ƽ�� ����Ʈ�� �ִٸ�,
	{
		//������ ����Ʈ �� ��ܿ� �߰��ϱ�.
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
		
		// ��Ƽ�� ����Ʈ ���� �ֱ�.
	}


	for (FQuestStruct Quest : QuestComponent->GetQuests())
	{
		if (Quest.QuestName == QuestComponent->GetActiveQuest().QuestName)
			continue;
		// ��Ƽ�� ����Ʈ�� ������ �߰������Ƿ� ���⼭ �ǳʶٱ�.

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
	if (!SlotArr.Num()) // 0��°�� ��Ƽ�� ����Ʈ�ε� ������ �� �ʿ䰡����.
		return;

	SlotArr[0]->BindQuestDescription(); // ������ ����ϸ�ȴ�.
}
