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

	// 트리거와 현재 캐릭터의 위치를 측정하는 함수.
	/*if (!ActiveQuest.QuestSteps.IsEmpty())
	{
		float Dist = OwnerCharacter->GetDistanceTo(Cast<AActor>(ActiveQuest.QuestSteps[0].Trigger));
		//GetDistanceTo 함수에서 알아서 매개변수로 들어온 값이 널인지 판단한다.
		// 이 로직으로 원신처럼 목표 위치랑 거리를 UI로 나타낼수 있다.
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
		//생성된 액터에 컴포넌트값 전달. 
		//퀘스트가 들어오면 해당 트리거를 생성
	}


	Quests.Add(NPCQuest);
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
	ActiveQuest.QuestSteps[0].Trigger->SetHiddenTriggerWidget(); // 먼저 열려있는 트리거의 틱과 위젯 비활성화.
	Quests[CurrentQuestID] = ActiveQuest; 
	//★★ 이렇게 다시 저장하고 리셋하면 퀘스트 진행도를 그대로 다시 저장가능하다.
	ResetQuest();	

	//빈칸이 아닐때 무언가 선택했다는 뜻.
	for (int32 i = 0; i < Quests.Num(); i++)
	{
		if (Quests[i].QuestName == SelectQuestName)
		{
			CurrentQuestID = i;
			ActiveQuest = Quests[i]; // 현재 실행하고 있는 퀘스트 실행.
			ActiveQuestChange.Broadcast();
		}
	}

	ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget();
}



// 퀘스트를 클리어 해서 삭제.
void UQuestComponent::RemoveQuest()
{
	Quests.RemoveAt(CurrentQuestID);
	SelectTopQuest(); // 다른 퀘스트가있으면 가장 상단에 있는 것이 활성화되고, 없으면 끝
	OnAddedRemovedQuest.Broadcast();
}

// 퀘스트 다음 단계
void UQuestComponent::CompleteStep()
{
	if (ActiveQuest.QuestSteps[0].Trigger)
		ActiveQuest.QuestSteps[0].Trigger->Destroy();

	//FinishedQuestNumber(ActiveQuest.QuestNumber); 
	// ★★ 완료한 퀘스트 넘버 수정. 하지만 이게임에서는 구현 안해도 된다. ★★

	ActiveQuest.QuestSteps.RemoveAt(0);
	if (ActiveQuest.QuestSteps.Num()) // 아직 존재하면,
	{
		ActiveQuest.bSucceed = false;
		ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget(); // 새로운 퀘스트 위젯 띄우기.
		CompleteQuestDelegate.Broadcast(); 
		// 다음 퀘스트를 가리켜야하니 관련 함수 호출. 
		// 퀘스트 캐시 업데이트와 실행중인 퀘스트 다음 퀘스트 설명으로 변경 함수가 바인딩 되어있다.
	}
	else
	{
		RemoveQuest();
	}

	UpdateQuestSave(); // 세이브 데이터 갱신.

}


void UQuestComponent::UpdateQuestCashes()
{
	QuestCashes.Empty();

	if (CurrentQuestID != -1)
		Quests[CurrentQuestID].QuestSteps = ActiveQuest.QuestSteps; // 퀘스트 진행도 저장.
	// 이 것으로 인해 세이브를 하고 로드 할때도 액티브 퀘스트가 한 퀘스트를 하나 진행했음에도 그 다음부터 바로 시작할 수 있게 해준다.

	for (FQuestStruct Quest : Quests) // 퀘스트 검색 용이하게 이름값으로 추가.
	{
		QuestCashes.Add(Quest.QuestName, Quest);
	}
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
		if (GI->QuestSave)
		{
			GI->QuestSave->SaveQuest(Quests, CurrentQuestID);
		}
	}
}

void UQuestComponent::StartQuestLoad()
{
	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this)); // 게임 인스턴스 저장.
	if (GI)// 게임 인스턴스가 존재 해야한다.
	{
		if (GI->QuestSave)
			GI->QuestSave->LoadQuest(this);
	}
}


// 몬스터나 횟수가 있는 퀘스트를 할때 현재 진행률 바인드
void UQuestComponent::BindSetDescription()
{
	SetDescriptionDelegate.Broadcast();
}


/* 완료한 퀘스트들을 나열할때 어떤 퀘스트가 완료한 것인지 판단하기위해 일일히 초기화 해주어야함.*/
/* 하지만 본 게임에서는 구현할 필요가 없기때문에 추후에 다른 게임에서는 참고하도록 하자. */
void UQuestComponent::FinishedQuestNumber(int32 QuestNumber)
{
	GI->FinishedQuestNumber(QuestNumber);
}

// NPC 이름까지 보내서 인스턴스 안에있는 TMap까지 제거 해도될듯 하다. ★★
// 굳이 이게임에서는 필요없으니 성능을 위해 구현하지 않음.
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

/* 퀘스트가 여러개있을때 다음 퀘스트 실행. */
void UQuestComponent::SelectTopQuest()
{
	if (Quests.Num())
	{
		CurrentQuestID = 0;
		ActiveQuest = Quests[0];
		ActiveQuest.QuestSteps[0].Trigger->SetTriggerWidget(); // 새로운 퀘스트 위젯 띄우기.
	}
	else
	{
		ResetQuest();
	}

}




