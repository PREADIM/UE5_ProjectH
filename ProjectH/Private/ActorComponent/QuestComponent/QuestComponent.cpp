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

			// 생성된 트리거에 컴포넌트값 전달. 
			// 퀘스트가 들어오면 해당 트리거를 생성
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

	OnAddedRemovedQuest.Broadcast(); // 퀘스트 캐쉬 & UI 초기화.
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

	// 이름이 같지 않을 시
	if (ActiveQuest.QuestSteps.Num())
	{
		ActiveQuest.QuestSteps[0].Trigger->SetHiddenTriggerWidget(); // 먼저 열려있는 트리거의 틱과 위젯 비활성화.
		Quests[CurrentQuestID] = ActiveQuest;
		//★★ 이렇게 다시 저장하고 리셋하면 퀘스트 진행도를 그대로 다시 저장가능하다.
	}
	ResetQuest();	

	//빈칸이 아닐때 무언가 선택했다는 뜻.
	for (int32 i = 0; i < Quests.Num(); i++)
	{
		if (Quests[i].QuestName == SelectQuestName)
		{
			CurrentQuestID = i;
			ActiveQuest = Quests[i]; // 현재 실행하고 있는 퀘스트 실행.
			ActiveQuestChange.Broadcast();
			break;
		}
	}

	ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget();
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
			GI->QuestClearNumber(Quests[QuestIndex].OwnerNPCName, RemoveQuestNumber);
			Quests.RemoveAt(QuestIndex);

			//RemoveAt을 했으므로 인덱스가 한칸씩 줄어든다.
			if (CurrentQuestID >= QuestIndex)
				--CurrentQuestID;

			SelectTopQuest(RemoveQuestNumber); // 실행중인 퀘였다면 다른 퀘스트가있으면 가장 상단에 있는 것이 활성화되고, 아니면 냅둠.
			OnAddedRemovedQuest.Broadcast();
			UpdateQuestSave();
		}
	}	
}

// 퀘스트 다음 단계
void UQuestComponent::CompleteStep()
{

	ATriggerEventBase* RemoveTrigger = ActiveQuest.QuestSteps[0].Trigger; // 
	if (!RemoveTrigger)
		return;

	ActiveQuest.QuestSteps.RemoveAt(0);
	if (ActiveQuest.QuestSteps.Num()) // 아직 존재하면,
	{
		ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget(); // 새로운 퀘스트 위젯 띄우기.
		CompleteQuestDelegate.Broadcast(); 
		// 다음 퀘스트를 가리켜야하니 관련 함수 호출. 
		// 퀘스트 캐시 업데이트와 실행중인 퀘스트 다음 퀘스트 설명으로 변경 함수가 바인딩 되어있다.
	}
	else
	{
		RemoveTrigger->ClearQuest(); // 퀘스트 클리어.
	}

	RemoveTrigger->TriggerDestroy();

	UpdateQuestSave(); // 세이브 데이터 갱신.
}


void UQuestComponent::UpdateQuestStep()
{
	if (CurrentQuestID != -1)
		Quests[CurrentQuestID].QuestSteps = ActiveQuest.QuestSteps; // 퀘스트 진행도 저장.
	// 이 것으로 인해 세이브를 하고 로드 할때도 액티브 퀘스트가 한 퀘스트를 하나 진행했음에도 그 다음부터 바로 시작할 수 있게 해준다.
}


//NPC -> MainUI -> QuestInfoUI ->QuestComponent 순이다.
void UQuestComponent::AcceptQuest()
{
	// ★★ AcceptDelegate에 연결되어있는 함수. 추후에 사용 가능. 퀘스트를 수락버튼 누르면 호출됨.

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
void UQuestComponent::BindSetDescription()
{
	SetDescriptionDelegate.Broadcast();
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
			ActiveQuest = Quests[0];
			ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget(); // 새로운 퀘스트 위젯 띄우기.
		}
	}
	else
	{
		ResetQuest();
	}

}




