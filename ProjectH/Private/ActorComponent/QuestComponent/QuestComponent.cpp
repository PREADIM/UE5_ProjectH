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

		// 생성된 트리거에 컴포넌트값 전달. 
		// 퀘스트가 들어오면 해당 트리거를 생성
	}

	Quests.Emplace(NPCQuest);
	HaveQuestNumber.Emplace(NPCQuest.QuestNumber);

	if (ActiveQuest.QuestName == FString(""))
	{
		ActiveQuest.SetActiveQuest(Quests[0]);
		CurrentQuestID = 0;
		Quests[CurrentQuestID].QuestSteps[0].Trigger->SetTriggerWidget();
	}

	OnUpdateQuestList.Broadcast(); // 퀘스트 캐쉬 & UI 초기화.
}

void UQuestComponent::SelectQuest(FString SelectQuestName)
{
	if (Quests.Num() == 0) // 퀘스트가 존재하지 않다면
	{
		ResetQuest();
		return;
	}

	if (ActiveQuest.QuestName == SelectQuestName) // 이름이 같으면 할 필요 없다.
		return;

	if (Quests[CurrentQuestID].QuestSteps.Num())
	{
		if(Quests[CurrentQuestID].QuestSteps[0].Trigger)
			Quests[CurrentQuestID].QuestSteps[0].Trigger->SetHiddenTriggerWidget(); // 먼저 열려있는 트리거의 틱과 위젯 비활성화.
	}

	ResetQuest();	

	//빈칸이 아닐때 무언가 선택했다는 뜻.
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



// 퀘스트를 클리어 해서 삭제.
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
					GI->AddCanQuest(NextQuestNumber); // 다음 퀘스트 추가.
					GI->PlaySequence(NextQuestNumber, OwnerController);
				}

			}
			Quests[QuestIndex].QuestSteps[0].Trigger->TriggerDestroy(); // 마지막 NPC 위치 나타내는 트리거 삭제. 필숨8★
			SaveQuestNumber(Quests[QuestIndex].OwnerNPCName, RemoveQuestNumber);
			Quests.RemoveAt(QuestIndex);

			//RemoveAt을 했으므로 인덱스가 한칸씩 줄어든다.
			if (CurrentQuestID >= QuestIndex)
				--CurrentQuestID;

			
			SelectTopQuest(RemoveQuestNumber); // 실행중인 퀘였다면 다른 퀘스트가있으면 가장 상단에 있는 것이 활성화되고, 아니면 냅둠.
			OnUpdateQuestList.Broadcast();
			UpdateQuestSave();
		}
	}	
}

// 퀘스트 다음 단계
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

	if (Quests[QuestIndex].QuestSteps.Num() > 1) // 마지막은 완료 NPC위치 트리거이므로 반드시 마지막엔 1개가 남아있어야함
	{
		Quests[QuestIndex].QuestSteps.RemoveAt(0);
		NewTriggerSet(QuestIndex); // 트리거 소환.
		if (Quests[QuestIndex].QuestSteps[0].Trigger)
		{
			RemoveTrigger->TriggerDestroy();
			Quests[QuestIndex].QuestSteps[0].Trigger->SetTriggerWidget();
			OnUpdateDescription.Broadcast(QuestIndex); // 해당 퀘스트의 설명이 바뀜.
		}
	}


	UpdateQuestSave(); // 세이브 데이터 갱신.
}


//NPC -> MainUI -> QuestInfoUI ->QuestComponent 순이다.
void UQuestComponent::AcceptQuest()
{
	UpdateQuestSave(); // 세이브 파일 갱신. 
}


void UQuestComponent::UpdateQuestSave()
{
	if (GI) // 이 함수는 StartQuestLoad 함수 뒤에 호출된다.
	{
		GI->SetSaveSlot(this);
	}
}

void UQuestComponent::StartQuestLoad()
{
	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this)); // 게임 인스턴스 저장.
	if (GI)// 게임 인스턴스가 존재 해야한다.
	{
		GI->SetLoadSlot(this);
		BeginSetupHaveQuests();
	}
}


// 몬스터나 횟수가 있는 퀘스트를 할때 현재 진행률 바인드
void UQuestComponent::BindSetDescription(int32 QuestIndex)
{
	OnUpdateDescription.Broadcast(QuestIndex);
}


// 퀘스트 완료를 했을때 호출될 함수.
// NPC 이름까지 보내서 인스턴스 안에있는 TMap까지 제거 한다.
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


// 퀘스트 스텝이 완료되었을때 그 다음 트리거 베이스를 생성.
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
		// 생성된 트리거에 컴포넌트값 전달. 
		// 퀘스트가 들어오면 해당 트리거를 생성
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

/* 퀘스트가 여러개있을때 다음 퀘스트 실행. */
void UQuestComponent::SelectTopQuest(int32 RemoveQuestNumber)
{
	if (Quests.Num())
	{
		// 현재 실행중인 퀘스트와 번호가 같다면
		if (ActiveQuest.QuestNumber == RemoveQuestNumber)
		{
			// RemoveAt으로 만일 0번째 인덱스를 지웠더라도 여기서 Number가 같은 것을 인지하고 
			// 0으로 만들어준다.

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




