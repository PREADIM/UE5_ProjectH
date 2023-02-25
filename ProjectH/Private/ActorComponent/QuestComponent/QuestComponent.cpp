// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "NPCQuest.h"
#include "QuestStruct.h"
#include "ActorComponent/QuestComponent/TriggerEventBase.h"
#include "Character/ProjectHCharacter.h"
#include "AI/QuestNPCBase.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Save/QuestSave.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	OnAddedRemovedQuest.AddUFunction(this, FName("UpdateQuestStep"));
	OwnerCharacter = Cast<class AProjectHCharacter>(GetOwner());
	CompleteQuestDelegate.AddUFunction(this, FName("UpdateQuestStep"));
	ActiveQuest.Clear();
	// ...
	
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



/*------------------
	public Function
--------------------*/

void UQuestComponent::AddQuest(FNPCQuest NPCQuest)
{
	if (HaveQuestNumber.Find(NPCQuest.QuestNumber))
	{
		_DEBUG("Have Quests");
		return;
	}

	for (int32 i = 0; i < NPCQuest.QuestSteps.Num(); i++)
	{
		NPCQuest.QuestSteps[i].Trigger = GetWorld()->SpawnActorDeferred<ATriggerEventBase>(NPCQuest.QuestSteps[i].BP_Trigger, FTransform(NPCQuest.QuestSteps[i].TriggerLocation), OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		if (::IsValid(NPCQuest.QuestSteps[i].Trigger))
		{
			NPCQuest.QuestSteps[i].Trigger->QuestComponent = this;
			NPCQuest.QuestSteps[i].Trigger->PlayerCharacter = Cast<AProjectHCharacter>(GetOwner());
			NPCQuest.QuestSteps[i].Trigger->GI = GI;
			NPCQuest.QuestSteps[i].Trigger->QuestName = NPCQuest.QuestName;
			NPCQuest.QuestSteps[i].Trigger->OwnerNPCName = NPCQuest.OwnerNPCName;
			NPCQuest.QuestSteps[i].Trigger->QuestNumber = NPCQuest.QuestNumber;

			NPCQuest.QuestSteps[i].Trigger->FinishSpawning(FTransform(NPCQuest.QuestSteps[i].TriggerLocation));

			// ������ Ʈ���ſ� ������Ʈ�� ����. 
			// ����Ʈ�� ������ �ش� Ʈ���Ÿ� ����
		}
	}

	Quests.Emplace(NPCQuest);
	HaveQuestNumber.Emplace(NPCQuest.QuestNumber);

	if (ActiveQuest.QuestName == FString(""))
	{
		ActiveQuest = Quests[0];
		CurrentQuestID = 0;
		ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget();
	}

	OnAddedRemovedQuest.Broadcast(); // ����Ʈ ĳ�� & UI �ʱ�ȭ.
}

void UQuestComponent::SelectQuest(FString SelectQuestName)
{
	if (Quests.Num() == 0) // ����Ʈ�� �������� �ʴٸ�
	{
		ResetQuest();
		return;
	}

	if (ActiveQuest.QuestName == SelectQuestName) // �̸��� ������ �� �ʿ� ����.
		return;

	// �̸��� ���� ���� ��
	if (ActiveQuest.QuestSteps.Num())
	{
		ActiveQuest.QuestSteps[0].Trigger->SetHiddenTriggerWidget(); // ���� �����ִ� Ʈ������ ƽ�� ���� ��Ȱ��ȭ.
		Quests[CurrentQuestID] = ActiveQuest;
		//�ڡ� �̷��� �ٽ� �����ϰ� �����ϸ� ����Ʈ ���൵�� �״�� �ٽ� ���尡���ϴ�.
	}
	ResetQuest();	

	//��ĭ�� �ƴҶ� ���� �����ߴٴ� ��.
	for (int32 i = 0; i < Quests.Num(); i++)
	{
		if (Quests[i].QuestName == SelectQuestName)
		{
			CurrentQuestID = i;
			ActiveQuest = Quests[i]; // ���� �����ϰ� �ִ� ����Ʈ ����.
			ActiveQuestChange.Broadcast();
			break;
		}
	}

	ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget();
}



// ����Ʈ�� Ŭ���� �ؼ� ����.
void UQuestComponent::RemoveQuest(int32 RemoveQuestNumber)
{
	if (GI)
	{
		int32 QuestIndex = -1;
		for (int32 i = 0; i < Quests.Num(); i++)
		{
			if (Quests[i].QuestNumber == RemoveQuestNumber)
			{
				QuestIndex = i;
				break;
			}
		}

		if (QuestIndex != -1)
		{
			GI->QuestClearNumber(Quests[QuestIndex].OwnerNPCName, RemoveQuestNumber);
			Quests.RemoveAt(QuestIndex);

			//RemoveAt�� �����Ƿ� �ε����� ��ĭ�� �پ���.
			if (CurrentQuestID >= QuestIndex)
				--CurrentQuestID;

			SelectTopQuest(RemoveQuestNumber); // �������� �����ٸ� �ٸ� ����Ʈ�������� ���� ��ܿ� �ִ� ���� Ȱ��ȭ�ǰ�, �ƴϸ� ����.
			OnAddedRemovedQuest.Broadcast();
			UpdateQuestSave();
		}
	}	
}

// ����Ʈ ���� �ܰ�
void UQuestComponent::CompleteStep()
{

	ATriggerEventBase* RemoveTrigger = ActiveQuest.QuestSteps[0].Trigger; // 
	if (!RemoveTrigger)
		return;

	ActiveQuest.QuestSteps.RemoveAt(0);
	if (ActiveQuest.QuestSteps.Num()) // ���� �����ϸ�,
	{
		ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget(); // ���ο� ����Ʈ ���� ����.
		CompleteQuestDelegate.Broadcast(); 
		// ���� ����Ʈ�� �����Ѿ��ϴ� ���� �Լ� ȣ��. 
		// ����Ʈ ĳ�� ������Ʈ�� �������� ����Ʈ ���� ����Ʈ �������� ���� �Լ��� ���ε� �Ǿ��ִ�.
	}
	else
	{
		RemoveTrigger->ClearQuest(); // ����Ʈ Ŭ����.
	}

	RemoveTrigger->TriggerDestroy();

	UpdateQuestSave(); // ���̺� ������ ����.
}


void UQuestComponent::UpdateQuestStep()
{
	if (CurrentQuestID != -1)
		Quests[CurrentQuestID].QuestSteps = ActiveQuest.QuestSteps; // ����Ʈ ���൵ ����.
	// �� ������ ���� ���̺긦 �ϰ� �ε� �Ҷ��� ��Ƽ�� ����Ʈ�� �� ����Ʈ�� �ϳ� ������������ �� �������� �ٷ� ������ �� �ְ� ���ش�.
}


//NPC -> MainUI -> QuestInfoUI ->QuestComponent ���̴�.
void UQuestComponent::AcceptQuest()
{
	// �ڡ� AcceptDelegate�� ����Ǿ��ִ� �Լ�. ���Ŀ� ��� ����. ����Ʈ�� ������ư ������ ȣ���.

	UpdateQuestSave(); // ���̺� ���� ����. 
}


void UQuestComponent::UpdateQuestSave()
{
	if (GI) // �� �Լ��� StartQuestLoad �Լ� �ڿ� ȣ��ȴ�.
	{
		GI->SetSaveSlot(this);
	}
}

void UQuestComponent::StartQuestLoad()
{
	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this)); // ���� �ν��Ͻ� ����.
	if (GI)// ���� �ν��Ͻ��� ���� �ؾ��Ѵ�.
	{
		GI->SetLoadSlot(this);
		BeginSetupHaveQuests();
	}
}


// ���ͳ� Ƚ���� �ִ� ����Ʈ�� �Ҷ� ���� ����� ���ε�
void UQuestComponent::BindSetDescription()
{
	SetDescriptionDelegate.Broadcast();
}


// ����Ʈ �ϷḦ ������ ȣ��� �Լ�.
// NPC �̸����� ������ �ν��Ͻ� �ȿ��ִ� TMap���� ���� �Ѵ�.
void UQuestComponent::SaveQuestNumber(FString NPCName, int32 QuestNumber)
{
	GI->QuestClearNumber(NPCName, QuestNumber);
}

void UQuestComponent::BeginSetupHaveQuests()
{
	for (FQuestStruct& Quest : Quests)
	{
		HaveQuestNumber.Emplace(Quest.QuestNumber);
	}

}

/* ----------------
	Private Value
------------------*/


void UQuestComponent::ResetQuest()
{
	CurrentQuestID = -1;
	ActiveQuest.Clear();
}

/* ����Ʈ�� ������������ ���� ����Ʈ ����. */
void UQuestComponent::SelectTopQuest(int32 RemoveQuestNumber)
{
	if (Quests.Num())
	{
		// ���� �������� ����Ʈ�� ��ȣ�� ���ٸ�
		if (ActiveQuest.QuestNumber == RemoveQuestNumber)
		{
			// RemoveAt���� ���� 0��° �ε����� �������� ���⼭ Number�� ���� ���� �����ϰ� 
			// 0���� ������ش�.
			CurrentQuestID = 0;
			ActiveQuest = Quests[0];
			ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget(); // ���ο� ����Ʈ ���� ����.
		}
	}
	else
	{
		ResetQuest();
	}

}




