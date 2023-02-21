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

	OnAddedRemovedQuest.AddUFunction(this, FName("UpdateQuestCashes"));
	OwnerCharacter = Cast<class AProjectHCharacter>(GetOwner());
	AcceptEvent.AddUFunction(this, FName("AcceptQuest"));
	CompleteQuestDelegate.AddUFunction(this, FName("UpdateQuestCashes"));
	ActiveQuest.Clear();
	// ...
	
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Ʈ���ſ� ���� ĳ������ ��ġ�� �����ϴ� �Լ�.
	/*if (!ActiveQuest.QuestSteps.IsEmpty())
	{
		float Dist = OwnerCharacter->GetDistanceTo(Cast<AActor>(ActiveQuest.QuestSteps[0].Trigger));
		//GetDistanceTo �Լ����� �˾Ƽ� �Ű������� ���� ���� ������ �Ǵ��Ѵ�.
		// �� �������� ����ó�� ��ǥ ��ġ�� �Ÿ��� UI�� ��Ÿ���� �ִ�.
		_DEBUG("%f", Dist / 100);

	}*/
}



/*------------------
	public Function
--------------------*/

void UQuestComponent::AddQuest(FNPCQuest NPCQuest)
{
	for (int32 i = 0; i < NPCQuest.QuestSteps.Num(); i++)
	{
		NPCQuest.QuestSteps[i].Trigger = GetWorld()->SpawnActor<ATriggerEventBase>(NPCQuest.QuestSteps[i].BP_Trigger, FTransform(NPCQuest.QuestSteps[i].TriggerLocation));
		if (::IsValid(NPCQuest.QuestSteps[i].Trigger))
		{
			NPCQuest.QuestSteps[i].Trigger->QuestComponent = this;
			NPCQuest.QuestSteps[i].Trigger->PlayerCharacter = Cast<AProjectHCharacter>(GetOwner());
			NPCQuest.QuestSteps[i].Trigger->QuestName = NPCQuest.QuestName;
		}
		//������ ���Ϳ� ������Ʈ�� ����. 
		//����Ʈ�� ������ �ش� Ʈ���Ÿ� ����
	}


	Quests.Add(NPCQuest);
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
	ActiveQuest.QuestSteps[0].Trigger->SetHiddenTriggerWidget(); // ���� �����ִ� Ʈ������ ƽ�� ���� ��Ȱ��ȭ.
	Quests[CurrentQuestID] = ActiveQuest; 
	//�ڡ� �̷��� �ٽ� �����ϰ� �����ϸ� ����Ʈ ���൵�� �״�� �ٽ� ���尡���ϴ�.
	ResetQuest();	

	//��ĭ�� �ƴҶ� ���� �����ߴٴ� ��.
	for (int32 i = 0; i < Quests.Num(); i++)
	{
		if (Quests[i].QuestName == SelectQuestName)
		{
			CurrentQuestID = i;
			ActiveQuest = Quests[i]; // ���� �����ϰ� �ִ� ����Ʈ ����.
			ActiveQuestChange.Broadcast();
		}
	}

	ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget();
}



// ����Ʈ�� Ŭ���� �ؼ� ����.
void UQuestComponent::RemoveQuest()
{
	Quests.RemoveAt(CurrentQuestID);
	SelectTopQuest(); // �ٸ� ����Ʈ�������� ���� ��ܿ� �ִ� ���� Ȱ��ȭ�ǰ�, ������ ��
	OnAddedRemovedQuest.Broadcast();
}

// ����Ʈ ���� �ܰ�
void UQuestComponent::CompleteStep()
{
	if (ActiveQuest.QuestSteps[0].Trigger)
		ActiveQuest.QuestSteps[0].Trigger->Destroy();

	//FinishedQuestNumber(ActiveQuest.QuestNumber); 
	// �ڡ� �Ϸ��� ����Ʈ �ѹ� ����. ������ �̰��ӿ����� ���� ���ص� �ȴ�. �ڡ�

	ActiveQuest.QuestSteps.RemoveAt(0);
	if (ActiveQuest.QuestSteps.Num()) // ���� �����ϸ�,
	{
		ActiveQuest.bSucceed = false;
		ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget(); // ���ο� ����Ʈ ���� ����.
		CompleteQuestDelegate.Broadcast(); 
		// ���� ����Ʈ�� �����Ѿ��ϴ� ���� �Լ� ȣ��. 
		// ����Ʈ ĳ�� ������Ʈ�� �������� ����Ʈ ���� ����Ʈ �������� ���� �Լ��� ���ε� �Ǿ��ִ�.
	}
	else
	{
		RemoveQuest();
	}

	UpdateQuestSave(); // ���̺� ������ ����.

}


void UQuestComponent::UpdateQuestCashes()
{
	QuestCashes.Empty();

	if (CurrentQuestID != -1)
		Quests[CurrentQuestID].QuestSteps = ActiveQuest.QuestSteps; // ����Ʈ ���൵ ����.
	// �� ������ ���� ���̺긦 �ϰ� �ε� �Ҷ��� ��Ƽ�� ����Ʈ�� �� ����Ʈ�� �ϳ� ������������ �� �������� �ٷ� ������ �� �ְ� ���ش�.

	for (FQuestStruct Quest : Quests) // ����Ʈ �˻� �����ϰ� �̸������� �߰�.
	{
		QuestCashes.Add(Quest.QuestName, Quest);
	}
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
		if (GI->QuestSave)
		{
			GI->QuestSave->SaveQuest(Quests, CurrentQuestID);
		}
	}
}

void UQuestComponent::StartQuestLoad()
{
	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this)); // ���� �ν��Ͻ� ����.
	if (GI)// ���� �ν��Ͻ��� ���� �ؾ��Ѵ�.
	{
		if (GI->QuestSave)
			GI->QuestSave->LoadQuest(this);
	}
}


// ���ͳ� Ƚ���� �ִ� ����Ʈ�� �Ҷ� ���� ����� ���ε�
void UQuestComponent::BindSetDescription()
{
	SetDescriptionDelegate.Broadcast();
}


/* �Ϸ��� ����Ʈ���� �����Ҷ� � ����Ʈ�� �Ϸ��� ������ �Ǵ��ϱ����� ������ �ʱ�ȭ ���־����.*/
/* ������ �� ���ӿ����� ������ �ʿ䰡 ���⶧���� ���Ŀ� �ٸ� ���ӿ����� �����ϵ��� ����. */
void UQuestComponent::FinishedQuestNumber(int32 QuestNumber)
{
	GI->FinishedQuestNumber(QuestNumber);
}

// NPC �̸����� ������ �ν��Ͻ� �ȿ��ִ� TMap���� ���� �ص��ɵ� �ϴ�. �ڡ�
// ���� �̰��ӿ����� �ʿ������ ������ ���� �������� ����.
void UQuestComponent::SaveQuestNumber(int32 QuestNumber)
{
	GI->AcceptQuestNumber(QuestNumber);
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
void UQuestComponent::SelectTopQuest()
{
	if (Quests.Num())
	{
		CurrentQuestID = 0;
		ActiveQuest = Quests[0];
		ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget(); // ���ο� ����Ʈ ���� ����.
	}
	else
	{
		ResetQuest();
	}

}




