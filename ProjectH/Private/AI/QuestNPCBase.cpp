// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/QuestNPCBase.h"
#include "NPCAllQuest.h"
#include "QuestStruct.h"
#include "ActorComponent/QuestComponent/TriggerEventBase.h"
#include "Controller/ProjectH_PC.h"
#include "Character/ProjectHCharacter.h"
#include "UI/QuestInfo.h"
#include "UI/DialogueWidget.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/NormalIconUI.h"
#include "UI/MainQuestUI.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/NormalIconUI.h"
#include "ActorComponent/QuestComponent/QuestingFunction.h"


// Sets default values
AQuestNPCBase::AQuestNPCBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	IconLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("IconLocation"));
	RootComponent = Root;
	IconLocationComponent->SetupAttachment(Root);

	QuestIconState = EQuestIconState::NONE;
	SetActorTickEnabled(false);
}

// Called when the game starts or when spawned
void AQuestNPCBase::BeginPlay()
{
	Super::BeginPlay();

	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		// NPC 이름으로 검색을해 퀘스트를 가져온다.
		FNPCQuestDataBase* AllQuest = GI->GetNPCQuestData(NPCName);
		if (AllQuest)
			NPCQuests = AllQuest->NPCAllQuests;

		//퀘스트 플래그들을 로드한다. ex) 퀘스트중인지, 퀘스트 완료가 있는지
		GI->SetNPCLoadSlot(this);
		GI->SetNPCPtr(NPCName, this);
	}

	if (!NPCQuests.Quests.Num()) // 없으면 그냥 리턴.
		return;
}

void AQuestNPCBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GI)
		SaveNPCQuest();

}

void AQuestNPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NPCIconSlot && PlayerController)
		PlayerController->MainQuestIconWidgetSetup(NPCIconSlot, IconLocationComponent->GetComponentLocation());

}

int32 AQuestNPCBase::RetIconPriority(EQuestIconState State)
{
	switch (State)
	{
	case EQuestIconState::MainSucceedQuest:
		return 6;
	case EQuestIconState::SubSucceedQuest:
		return 5;
	case EQuestIconState::MainCanQuest:
		return 4;
	case EQuestIconState::SubCanQuest:
		return 3;
	case EQuestIconState::MainQuesting:
		return 2;
	case EQuestIconState::SubQuesting:
		return 1;
	default:
		return 0;
	}
}

void AQuestNPCBase::SetQuestIconState(EQuestIconState NewState)
{
	int32 Current = RetIconPriority(QuestIconState);
	int32 New = RetIconPriority(NewState);

	if (Current < New) // 새로운게 우선순위가 더 크면 교체.
		QuestIconState = NewState;

}

/*-------------------
	Public Function
--------------------*/


void AQuestNPCBase::Interact_Implementation(class AProjectHCharacter* OwnerCharacter)
{
	BPBindInteract();
}


void AQuestNPCBase::QuestInfoOpen(int32 QuestIndex, AProjectH_PC* OwnerController)
{
	//NPCQuests의 Inex 번호로 해당 퀘스트 창을 띄운다.
	//나중에 텍스트를 넘기고 마지막에 퀘스트를 고르는 ui를 구현하기 위해 나눔. (ex.로스트아크)
	if (!OwnerController && !NPCQuests.Quests.IsValidIndex(QuestIndex))
		return;

	OwnerController->MainQuestUI->OpenInfoUI(NPCQuests.Quests[QuestIndex], QuestIndex);
}


//완료 창을 따로 띄운다. Info와 거의 같다.
void AQuestNPCBase::QuestSucceedInfoOpen(int32 QuestIndex, AProjectH_PC* OwnerController)
{
	if (!OwnerController && !NPCQuests.Quests.IsValidIndex(QuestIndex))
		return;

	OwnerController->MainQuestUI->OpenSucceedInfo(NPCQuests.Quests[QuestIndex], QuestIndex);
}


void AQuestNPCBase::NPCQuestSetup()
{
	// 여기서 받아오는 bool값을 이용해서 인터랙트 아이콘을 변경해도 될듯하다.
	bool bHaveCanQuest = FindCanQuest();
	SetIconWidget();
	SaveNPCQuest();
}

void AQuestNPCBase::SaveNPCQuest()
{
	GI->SetNPCSaveSlot(this);
	/* NPC 이름, NPC 퀘스트 목록, 퀘스트 완료상태, 퀘스트 중인지 상태, 퀘스트를 수락할수 있는지 상태 */
}


void AQuestNPCBase::SetIconWidget()
{
	if (!NPCQuestIconUI)
	{
		if (!PlayerController)
			return;

		NPCQuestIconUI = CreateWidget<UNormalIconUI>(GetWorld(), BP_NPCQuestIconUI);
		if (NPCQuestIconUI)
			NPCIconSlot = PlayerController->AddChildCanvas(NPCQuestIconUI);
			
	}

	switch (QuestIconState)
	{
	case EQuestIconState::NONE:
		NPCQuestIconUI->SetRenderIcon(EQuestState::NONE);
		NPCQuestIconUI->SetRenderOpacity(0.f);
		return; // 여기서는 리턴이다.

	case EQuestIconState::SubCanQuest:
		NPCQuestIconUI->SetRenderIcon(EQuestState::SubQuest);
		break;
	case EQuestIconState::MainCanQuest:
		NPCQuestIconUI->SetRenderIcon(EQuestState::MainQuest);
		break;
	case EQuestIconState::SubQuesting:
		NPCQuestIconUI->SetRenderIcon(EQuestState::SubQuesting);
		break;
	case EQuestIconState::MainQuesting:
		NPCQuestIconUI->SetRenderIcon(EQuestState::MainQuesting);
		break;
	case EQuestIconState::SubSucceedQuest:
		NPCQuestIconUI->SetRenderIcon(EQuestState::SubSucceedQuest);
		break;
	case EQuestIconState::MainSucceedQuest:
		NPCQuestIconUI->SetRenderIcon(EQuestState::MainSucceedQuest);
		break;
	}

	if(NPCQuestIconUI->GetRenderOpacity() != 1.f)
		NPCQuestIconUI->SetRenderOpacity(1.f);

	SetActorTickEnabled(true);
}

void AQuestNPCBase::HiddenIcon()
{
	if (!NPCQuestIconUI)
		return;

	NPCQuestIconUI->SetRenderOpacity(0.f);
	SetActorTickEnabled(false);
}


/* 블루프린트에서 완료한 퀘스트 접근 */
bool AQuestNPCBase::FindEndedQuestsNums(int32 QuestNumber)
{
	int32* EndedQuestNumber = EndedQuestsNums.Find(QuestNumber);
	if (EndedQuestNumber)
		return true;
	else
		return false;
}


/* 캐릭터가 해당 NPC의 퀘스트를 수행조건이 가능한지 검사한다. */
bool AQuestNPCBase::FindCanQuest()
{
	// 게임 인스턴스 조회는 NPC 이름으로 하면 될듯하다.
	/*  게임 인스턴스 같은 곳에서 캐릭터가 수행가능한 퀘스트 목록을 퀘스트 이름으로 저장하고
		여기서 그 목록을 조회해 해당 NPC가 수행가능한 퀘스트를 가지고 있으면 해당 퀘스트의
		bCanAccepted를 true하고 NPC 클래스의 bCanInteractQuest를 true로 바꾼다.
		해당 NPC 클래스의 bCanInteractQuest는 하나라도 수락 가능한 퀘스트가 있다는 뜻이고,
		★ NPCQuest의 bCanAccepted는 다이얼 로그 후에 나열할 퀘스트 목록을 할떄 사용.
		또한 해당 퀘스트가 진행중인지, 완료가능인지 완료된 것인지 판단하여 퀘스트를 지우고, 플래그를 설정한다.
		*/

	//여기서 항상 NONE으로 기준을 잡고 하면 된다.

	QuestIconState = EQuestIconState::NONE;

	if (GI)
	{
		FCanQuestNums* CanQuestNum = GI->PlayerCanQuest.Find(NPCName);
		if (CanQuestNum)
		{
			for (int32 i = 0; i < NPCQuests.Quests.Num(); ++i)
			{
				int32* EndedNum = EndedQuestsNums.Find(NPCQuests.Quests[i].QuestNumber);
				if (EndedNum)
				{			
					NPCQuests.Quests.RemoveAt(i); // 완료한 것들 찾아서 삭제.
					--i; // 지우면서 엘리멘트가 앞으로 당겨졌으므로.
					continue;
				}
				else
				{
					// 퀘스팅 커스텀 함수가 있는경우. 퀘스팅중 이걸 선택하면 어떤 함수를 실행하는 방식.
					if (NPCQuests.Quests[i].BP_QuestingFunction)
					{
						if (NPCQuests.Quests[i].QuestingFunction == nullptr)
						{
							UQuestingFunction* temp = NewObject<UQuestingFunction>(this, NPCQuests.Quests[i].BP_QuestingFunction);
							if (temp)
							{
								temp->OwnerNPC = this;
								temp->GI = GI;
								NPCQuests.Quests[i].QuestingFunction = temp;
							}
						}
						
					}


					// 완료가능한 퀘스트 찾기.		
					if (int32* SucceedQuestNum = SucceedQuestsNums.Find(NPCQuests.Quests[i].QuestNumber))
					{
						NPCQuests.Quests[i].CanSucceed = true;
						NPCQuests.Quests[i].bCanAccepted = true;

						switch (NPCQuests.Quests[i].QuestType)
						{
						case EQuestType::Main :
							SetQuestIconState(EQuestIconState::MainSucceedQuest);
							break;
						case EQuestType::Normal :
							SetQuestIconState(EQuestIconState::SubSucceedQuest);
							break;
						}
					}
					else if(int32* QuestingNum = QuestingNums.Find(NPCQuests.Quests[i].QuestNumber))
					{		
						NPCQuests.Quests[i].Questing = true;
						NPCQuests.Quests[i].bCanAccepted = true;

						switch (NPCQuests.Quests[i].QuestType)
						{
						case EQuestType::Main:
							SetQuestIconState(EQuestIconState::MainQuesting);
							break;
						case EQuestType::Normal:
							SetQuestIconState(EQuestIconState::SubQuesting);
							break;
						}
					}					
					else // 진행중도 아니고 완료가능한 퀘스트도 아닌경우. 
					{
						// 유저가 가능한 퀘스트 찾아서 퀘스트 가능하다고 체크해주기.		
						if (int32* CanNum = CanQuestNum->QuestNums.Find(NPCQuests.Quests[i].QuestNumber))
						{
							NPCQuests.Quests[i].bCanAccepted = true;

							switch (NPCQuests.Quests[i].QuestType)
							{
							case EQuestType::Main:
								SetQuestIconState(EQuestIconState::MainCanQuest);
								break;
							case EQuestType::Normal:
								SetQuestIconState(EQuestIconState::SubCanQuest);
								break;
							}
						}
					}			
				}
			}
			return true;
		}
	}
	return false;
}
