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
//#include "Save/QuestSave.h"
#include "UI/NormalIconUI.h"
#include "UI/MainQuestUI.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/NormalIconUI.h"
#include "ActorComponent/QuestComponent/QuestingFunction.h"


// Sets default values
AQuestNPCBase::AQuestNPCBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;


	QuestIconComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuestIconComponent"));
	QuestIconComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UNormalIconUI> BP_Icon(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PROJECT/BP_CLASS/Blueprints/04_Special/BP_QuestSystem/BP_QuestIconWidget'"));
	if (BP_Icon.Succeeded())
	{
		QuestIconComponent->SetWidgetClass(BP_Icon.Class);
		QuestIconComponent->SetDrawSize(FVector2D(100.f, 100.f));
	}

	QuestIconComponent->SetVisibility(false);
	QuestIconState = EQuestIconState::NONE;
}

// Called when the game starts or when spawned
void AQuestNPCBase::BeginPlay()
{
	Super::BeginPlay();

	QuestIconComponent->InitWidget();
	QuestIconUI = Cast<UNormalIconUI>(QuestIconComponent->GetUserWidgetObject());

	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		// NPC �̸����� �˻����� ����Ʈ�� �����´�.
		FNPCQuestDataBase* AllQuest = GI->GetNPCQuestData(NPCName);
		if (AllQuest)
			NPCQuests = AllQuest->NPCAllQuests;

		//����Ʈ �÷��׵��� �ε��Ѵ�. ex) ����Ʈ������, ����Ʈ �Ϸᰡ �ִ���
		if (!GI->SetNPCLoadSlot(this))
			_DEBUG("false LoadNPC");

		GI->SetNPCPtr(NPCName, this);
	}

	if (!NPCQuests.Quests.Num()) // ������ �׳� ����.
		return;
}

void AQuestNPCBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GI)
		SaveNPCQuest();

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

	if (Current < New) // ���ο�� �켱������ �� ũ�� ��ü.
		QuestIconState = NewState;


}

/*-------------------
	Public Function
--------------------*/


void AQuestNPCBase::Interact_Implementation(class AProjectHCharacter* OwnerCharacter)
{
}


void AQuestNPCBase::QuestInfoOpen(int32 QuestIndex, AProjectH_PC* OwnerController)
{
	//NPCQuests�� Inex ��ȣ�� �ش� ����Ʈ â�� ����.
	//���߿� �ؽ�Ʈ�� �ѱ�� �������� ����Ʈ�� ���� ui�� �����ϱ� ���� ����. (ex.�ν�Ʈ��ũ)
	if (!OwnerController && !NPCQuests.Quests.IsValidIndex(QuestIndex))
		return;

	OwnerController->MainQuestUI->OpenInfoUI(NPCQuests.Quests[QuestIndex], QuestIndex);
}


//�Ϸ� â�� ���� ����. Info�� ���� ����.
void AQuestNPCBase::QuestSucceedInfoOpen(int32 QuestIndex, AProjectH_PC* OwnerController)
{
	if (!OwnerController && !NPCQuests.Quests.IsValidIndex(QuestIndex))
		return;

	OwnerController->MainQuestUI->OpenSucceedInfo(NPCQuests.Quests[QuestIndex], QuestIndex);
}


void AQuestNPCBase::NPCQuestSetup()
{
	// ���⼭ �޾ƿ��� bool���� �̿��ؼ� ���ͷ�Ʈ �������� �����ص� �ɵ��ϴ�.
	bool bHaveCanQuest = FindCanQuest();
	SetIconWidget();
	SaveNPCQuest();
}

void AQuestNPCBase::SaveNPCQuest()
{
	GI->SetNPCSaveSlot(this);
	/* NPC �̸�, NPC ����Ʈ ���, ����Ʈ �Ϸ����, ����Ʈ ������ ����, ����Ʈ�� �����Ҽ� �ִ��� ���� */
}


void AQuestNPCBase::SetIconWidget()
{
	if (!QuestIconUI)
		return;

	switch (QuestIconState)
	{
	case EQuestIconState::NONE:
		QuestIconUI->SetRenderIcon(EQuestState::NONE);
		QuestIconComponent->SetVisibility(false);
		return; // ���⼭�� �����̴�.
	case EQuestIconState::SubCanQuest:
		QuestIconUI->SetRenderIcon(EQuestState::SubQuest);
		break;
	case EQuestIconState::MainCanQuest:
		QuestIconUI->SetRenderIcon(EQuestState::MainQuest);
		break;
	case EQuestIconState::SubQuesting:
		QuestIconUI->SetRenderIcon(EQuestState::SubQuesting);
		break;
	case EQuestIconState::MainQuesting:
		QuestIconUI->SetRenderIcon(EQuestState::MainQuesting);
		break;
	case EQuestIconState::SubSucceedQuest:
		QuestIconUI->SetRenderIcon(EQuestState::SubSucceedQuest);
		break;
	case EQuestIconState::MainSucceedQuest:
		QuestIconUI->SetRenderIcon(EQuestState::MainSucceedQuest);
		break;
	}

	QuestIconComponent->SetVisibility(true);
}

void AQuestNPCBase::HiddenIcon()
{
	QuestIconComponent->SetVisibility(false);
	_DEBUG("HiddenIcon");
}


/* ĳ���Ͱ� �ش� NPC�� ����Ʈ�� ���������� �������� �˻��Ѵ�. */
bool AQuestNPCBase::FindCanQuest()
{
	// ���� �ν��Ͻ� ��ȸ�� NPC �̸����� �ϸ� �ɵ��ϴ�.
	/*  ���� �ν��Ͻ� ���� ������ ĳ���Ͱ� ���డ���� ����Ʈ ����� ����Ʈ �̸����� �����ϰ�
		���⼭ �� ����� ��ȸ�� �ش� NPC�� ���డ���� ����Ʈ�� ������ ������ �ش� ����Ʈ��
		bCanAccepted�� true�ϰ� NPC Ŭ������ bCanInteractQuest�� true�� �ٲ۴�.
		�ش� NPC Ŭ������ bCanInteractQuest�� �ϳ��� ���� ������ ����Ʈ�� �ִٴ� ���̰�,
		�� NPCQuest�� bCanAccepted�� ���̾� �α� �Ŀ� ������ ����Ʈ ����� �ҋ� ���.
		���� �ش� ����Ʈ�� ����������, �Ϸᰡ������ �Ϸ�� ������ �Ǵ��Ͽ� ����Ʈ�� �����, �÷��׸� �����Ѵ�.
		*/


	//���⼭ �׻� NONE���� ������ ��� �ϸ� �ȴ�.��
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
					NPCQuests.Quests.RemoveAt(i); // �Ϸ��� �͵� ã�Ƽ� ����.
					--i; // ����鼭 ������Ʈ�� ������ ��������Ƿ�.
					continue;
				}
				else
				{
					// ������ Ŀ���� �Լ��� �ִ°��. �������� �̰� �����ϸ� � �Լ��� �����ϴ� ���.
					if (NPCQuests.Quests[i].BP_QuestingFunction)
					{
						if (NPCQuests.Quests[i].QuestingFunction == nullptr)
						{
							UQuestingFunction* temp = NewObject<UQuestingFunction>(this, NPCQuests.Quests[i].BP_QuestingFunction);
							//UQuestingFunction* temp = Cast<UQuestingFunction>(NPCQuests.Quests[i].BP_QuestingFunction->GetDefaultObject());
							if (temp)
							{
								temp->OwnerNPC = this;
								temp->GI = GI;
								NPCQuests.Quests[i].QuestingFunction = temp;
							}
						}
						
					}


					// �Ϸᰡ���� ����Ʈ ã��.		
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
					else // �����ߵ� �ƴϰ� �Ϸᰡ���� ����Ʈ�� �ƴѰ��. 
					{
						// ������ ������ ����Ʈ ã�Ƽ� ����Ʈ �����ϴٰ� üũ���ֱ�.		
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
