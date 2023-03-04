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


FActiveQuest::FActiveQuest()
{
	Clear();
}


void FActiveQuest::Clear()
{
	QuestName = FString("");
	QuestNumber = -1;
	OwnerNPCName = FString("");
}


void FActiveQuest::SetActiveQuest(FQuestStruct& Quest)
{
	QuestName = Quest.QuestName;
	QuestNumber = Quest.QuestNumber;
	OwnerNPCName = Quest.OwnerNPCName;
}



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

	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<class AProjectHCharacter>(GetOwner());	
	}
			
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
		return;
	}

	if (!OwnerCharacter || !OwnerController)
	{
		_DEBUG("Not OwnerCharacter || OwnerController");
		return;
	}


	NPCQuest.QuestSteps[0].Trigger = GetWorld()->SpawnActorDeferred<ATriggerEventBase>(NPCQuest.QuestSteps[0].BP_Trigger, FTransform(NPCQuest.QuestSteps[0].TriggerLocation), OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (::IsValid(NPCQuest.QuestSteps[0].Trigger))
	{
		//_DEBUG("Trigger Name : %s", *NPCQuest.QuestSteps[0].Trigger->GetName());
		NPCQuest.QuestSteps[0].Trigger->QuestComponent = this;
		NPCQuest.QuestSteps[0].Trigger->PlayerCharacter = OwnerCharacter;
		NPCQuest.QuestSteps[0].Trigger->PlayerController = OwnerController;
		NPCQuest.QuestSteps[0].Trigger->GI = GI;
		NPCQuest.QuestSteps[0].Trigger->QuestName = NPCQuest.QuestName;
		NPCQuest.QuestSteps[0].Trigger->OwnerNPCName = NPCQuest.OwnerNPCName;
		NPCQuest.QuestSteps[0].Trigger->QuestNumber = NPCQuest.QuestNumber;

		NPCQuest.QuestSteps[0].Trigger->FinishSpawning(FTransform(NPCQuest.QuestSteps[0].TriggerLocation));

		// ������ Ʈ���ſ� ������Ʈ�� ����. 
		// ����Ʈ�� ������ �ش� Ʈ���Ÿ� ����
	}

	Quests.Emplace(NPCQuest);
	HaveQuestNumber.Emplace(NPCQuest.QuestNumber);

	if (ActiveQuest.QuestName == FString(""))
	{
		ActiveQuest.SetActiveQuest(Quests[0]);
		CurrentQuestID = 0;
		Quests[CurrentQuestID].QuestSteps[0].Trigger->SetTriggerWidget();
	}

	OnUpdateQuestList.Broadcast(); // ����Ʈ ĳ�� & UI �ʱ�ȭ.
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

	if (Quests[CurrentQuestID].QuestSteps.Num())
	{
		if(Quests[CurrentQuestID].QuestSteps[0].Trigger)
			Quests[CurrentQuestID].QuestSteps[0].Trigger->SetHiddenTriggerWidget(); // ���� �����ִ� Ʈ������ ƽ�� ���� ��Ȱ��ȭ.
	}

	ResetQuest();	

	//��ĭ�� �ƴҶ� ���� �����ߴٴ� ��.
	for (int32 i = 0; i < Quests.Num(); i++)
	{
		if (Quests[i].QuestName == SelectQuestName)
		{
			CurrentQuestID = i;
			ActiveQuest.SetActiveQuest(Quests[i]);
			if (Quests[CurrentQuestID].QuestSteps.Num() && Quests[CurrentQuestID].QuestSteps[0].Trigger)
			{
				Quests[CurrentQuestID].QuestSteps[0].Trigger->SetTriggerWidget();
			}
				
			OnUpdateQuestList.Broadcast();
			break;
		}
	}
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
			if (Quests[QuestIndex].ClearNextQuest.Num())
			{
				for (int32& NextQuestNumber : Quests[QuestIndex].ClearNextQuest)
				{
					GI->AddCanQuest(NextQuestNumber); // ���� ����Ʈ �߰�.
					GI->PlaySequence(NextQuestNumber, OwnerController);
				}

			}
			Quests[QuestIndex].QuestSteps[0].Trigger->TriggerDestroy(); // ������ NPC ��ġ ��Ÿ���� Ʈ���� ����. �ʼ�8��
			SaveQuestNumber(Quests[QuestIndex].OwnerNPCName, RemoveQuestNumber);
			Quests.RemoveAt(QuestIndex);

			//RemoveAt�� �����Ƿ� �ε����� ��ĭ�� �پ���.
			if (CurrentQuestID >= QuestIndex)
				--CurrentQuestID;

			
			SelectTopQuest(RemoveQuestNumber); // �������� �����ٸ� �ٸ� ����Ʈ�������� ���� ��ܿ� �ִ� ���� Ȱ��ȭ�ǰ�, �ƴϸ� ����.
			OnUpdateQuestList.Broadcast();
			UpdateQuestSave();
		}
	}	
}

// ����Ʈ ���� �ܰ�
void UQuestComponent::CompleteStep(int32 QuestNumber)
{
	int32 QuestIndex = -1;
	for (int32 i = 0; i < Quests.Num(); ++i)
	{
		if (QuestNumber == Quests[i].QuestNumber)
		{
			QuestIndex = i;
			break;
		}
	}

	if (QuestIndex == -1)
		return;

	ATriggerEventBase* RemoveTrigger = Quests[QuestIndex].QuestSteps[0].Trigger; // 
	if (!RemoveTrigger)
		return;

	if (Quests[QuestIndex].QuestSteps.Num() > 1) // �������� �Ϸ� NPC��ġ Ʈ�����̹Ƿ� �ݵ�� �������� 1���� �����־����
	{
		Quests[QuestIndex].QuestSteps.RemoveAt(0);
		NewTriggerSet(QuestIndex); // Ʈ���� ��ȯ.
		if (Quests[QuestIndex].QuestSteps[0].Trigger)
		{
			RemoveTrigger->TriggerDestroy();
			Quests[QuestIndex].QuestSteps[0].Trigger->SetTriggerWidget();
			OnUpdateDescription.Broadcast(QuestIndex); // �ش� ����Ʈ�� ������ �ٲ�.
		}
	}


	UpdateQuestSave(); // ���̺� ������ ����.
}


//NPC -> MainUI -> QuestInfoUI ->QuestComponent ���̴�.
void UQuestComponent::AcceptQuest()
{
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
void UQuestComponent::BindSetDescription(int32 QuestIndex)
{
	OnUpdateDescription.Broadcast(QuestIndex);
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


// ����Ʈ ������ �Ϸ�Ǿ����� �� ���� Ʈ���� ���̽��� ����.
void UQuestComponent::NewTriggerSet(int32 QuestIndex)
{
	Quests[QuestIndex].QuestSteps[0].Trigger = GetWorld()->SpawnActorDeferred<ATriggerEventBase>(Quests[QuestIndex].QuestSteps[0].BP_Trigger, FTransform(Quests[QuestIndex].QuestSteps[0].TriggerLocation), OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
	if (::IsValid(Quests[QuestIndex].QuestSteps[0].Trigger))
	{
		Quests[QuestIndex].QuestSteps[0].Trigger->QuestComponent = this;
		Quests[QuestIndex].QuestSteps[0].Trigger->PlayerCharacter = Cast<AProjectHCharacter>(GetOwner());
		Quests[QuestIndex].QuestSteps[0].Trigger->GI = GI;
		Quests[QuestIndex].QuestSteps[0].Trigger->QuestName = Quests[QuestIndex].QuestName;
		Quests[QuestIndex].QuestSteps[0].Trigger->OwnerNPCName = Quests[QuestIndex].OwnerNPCName;
		Quests[QuestIndex].QuestSteps[0].Trigger->QuestNumber = Quests[QuestIndex].QuestNumber;

		Quests[QuestIndex].QuestSteps[0].Trigger->FinishSpawning(FTransform(Quests[QuestIndex].QuestSteps[0].TriggerLocation));
		// ������ Ʈ���ſ� ������Ʈ�� ����. 
		// ����Ʈ�� ������ �ش� Ʈ���Ÿ� ����
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
			ActiveQuest.SetActiveQuest(Quests[0]);
			Quests[CurrentQuestID].QuestSteps[0].Trigger->SetTriggerWidget();
		}
	}
	else
	{
		ResetQuest();
	}

}




