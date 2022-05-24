// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/QuestNPCBase.h"
#include "NPCAllQuest.h"
#include "QuestStruct.h"
#include "ActorComponent/QuestComponent/TriggerEventBase.h"
#include "Controller/ProjectH_PC.h"
#include "Character/ProjectHCharacter.h"
#include "UI/MainQuestUI.h"
#include "UI/QuestInfo.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Save/QuestSave.h"
#include "UI/NormalIconUI.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AQuestNPCBase::AQuestNPCBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	IconWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("IconWidget"));
	MainIconWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MainIconWidget"));
	SucceedWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SucceedWidget"));

	RootComponent = CapsuleCollision;
	Mesh->SetupAttachment(CapsuleCollision);
	IconWidget->SetupAttachment(Mesh);
	MainIconWidget->SetupAttachment(Mesh);
	SucceedWidget->SetupAttachment(Mesh);

	static ConstructorHelpers::FClassFinder<UNormalIconUI> BP_Icon(TEXT("WidgetBlueprint'/Game/PROJECT/BP_CLASS/Blueprints/04_Special/BP_QuestSystem/BP_QuestIcon'"));
	if (BP_Icon.Succeeded())
	{
		IconWidget->SetWidgetClass(BP_Icon.Class);
		IconWidget->SetDrawSize(FVector2D(100.f, 100.f));
	}

	static ConstructorHelpers::FClassFinder<UNormalIconUI> BP_MainIcon(TEXT("WidgetBlueprint'/Game/PROJECT/BP_CLASS/Blueprints/04_Special/BP_QuestSystem/BP_MainQuestIcon'"));
	if (BP_MainIcon.Succeeded())
	{
		MainIconWidget->SetWidgetClass(BP_MainIcon.Class);
		MainIconWidget->SetDrawSize(FVector2D(100.f, 100.f));
	}

	static ConstructorHelpers::FClassFinder<UNormalIconUI> BP_SucceedIcon(TEXT("WidgetBlueprint'/Game/PROJECT/BP_CLASS/Blueprints/04_Special/BP_QuestSystem/BP_QuestIcon_Succeed'"));
	if (BP_SucceedIcon.Succeeded())
	{
		SucceedWidget->SetWidgetClass(BP_SucceedIcon.Class);
		SucceedWidget->SetDrawSize(FVector2D(100.f, 100.f));
	}

	IconWidget->SetVisibility(false);
	MainIconWidget->SetVisibility(false);
	SucceedWidget->SetVisibility(false);


	bQuestSucceed = false;
	bIsQuesting = false;
	bCanAccept = false;
	bHaveMainQuest = false;
	CanQuestCnt = 0;
	CanMainQuestCnt = 0;
}

// Called when the game starts or when spawned
void AQuestNPCBase::BeginPlay()
{
	Super::BeginPlay();

	IconWidget->InitWidget();
	MainIconWidget->InitWidget();
	SucceedWidget->InitWidget();

	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		bool bFlag = GI->QuestSave->LoadNPC(this);
		if (!bFlag)
		{
			/* false �� �ε� �Ұ� ����. true�� �����Ͱ� ����.*/
			FindCanQuest(); // ���� �ν��Ͻ����� ����Ʈ ��� �˻�. �ڼ��Ѱ��� �ش� �Լ��� �����.
		}
	}


	if (!NPCQuests.Quests.Num()) // ������ �׳� ����.
		return;

	for (int32 i = 0; i < NPCQuests.Quests.Num(); i++)
	{
		NPCQuests.Quests[i].OwnerNPC = this;
	} //������ ���⼭ Ʈ������ ������ �����ߴ�. ���忡 �̸� ��ġ�� Ʈ���Ÿ� ���⼭ ����.


	SaveNPCQuest();
}

void AQuestNPCBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GI)
		SaveNPCQuest();

}



// Called every frame
void AQuestNPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PlayerCharacter != nullptr)
	{
		FVector V = GetActorLocation() - PlayerCharacter->GetActorLocation();
		float Range = UKismetMathLibrary::MapRangeUnclamped(V.Length(), 100, 6000, 1.0, 0.2);
		if (bQuestSucceed)
		{
			SucceedWidget->GetUserWidgetObject()->SetRenderScale(FVector2D(Range, Range));
		}
		else if (bHaveMainQuest)
		{
			MainIconWidget->GetUserWidgetObject()->SetRenderScale(FVector2D(Range, Range));
		}
		else
		{
			IconWidget->GetUserWidgetObject()->SetRenderScale(FVector2D(Range, Range));
		}
	}
}


/*-------------------
	Public Function
--------------------*/

/* �ǽð����� NPC�� ����Ʈ �߰��ϴ� ���. �Ϸ� ����Ʈ�� �ᱹ�� ����Ʈ �̱⶧���� �̷������� �߰� ����.*/
void AQuestNPCBase::AddNPCQuest(FNPCQuest Quest)
{
	//FindCanQuest(); 
	CanQuestCnt++;
	bCanAccept = true;
	//bQuestSucceed = true; �̷������� �߰��� ���ÿ� Succeed�� �ϸ� �ٷ� �Ϸ�Ȱ� �����ϰ� �Ҽ� �������ϴ�.
}


void AQuestNPCBase::Interact_Implementation(class AProjectHCharacter* OwnerCharacter)
{
	_DEBUG("HI");
	// ���⼭ ���̾� �α׸� ����� �ɵ��ϴ�.
	// ���̾� �α� �������� ��ư���� ������ �ִ� ����Ʈ�� ���� �ش�����Ʈ ��ư�� ������ QuestInfoOpen�Լ��� ��ȣ�� ��������.
	// ������ ������ 0 ������.
	QuestInfoOpen(0, Cast<AProjectH_PC>(OwnerCharacter->GetController()));
	// Ŭ���Ҽ��ִ� ����Ʈ�� ������ �ϼ��������� �ε��� ��ȣ�� , ��Ʈ�ѷ� ��ȯ.
}


void AQuestNPCBase::QuestInfoOpen(int32 QuestIndex, AProjectH_PC* OwnerController)
{
	//NPCQuests�� Inex ��ȣ�� �ش� ����Ʈ â�� ����.
	//���߿� �ؽ�Ʈ�� �ѱ�� �������� ����Ʈ�� ���� ui�� �����ϱ� ���� ����. (ex.�ν�Ʈ��ũ)

	if (!OwnerController && !NPCQuests.Quests.IsValidIndex(QuestIndex))
	{
		return;
	}

	OwnerController->MainQuestUI->OpenInfoUI(NPCQuests.Quests[QuestIndex], QuestIndex);
	// ���⼭ �ƿ� QuestInfo�� ����Ʈ �ε��� ��, ����Ʈ �̸�, ����Ʈ ������ �����ص� ������,
	// MainQuest�� Owner ��Ʈ�ѷ��� ����Ʈ ������Ʈ�� �߰��ؾ��ϱ⶧���� Main���� �ذ��Ѵ�.
}

void AQuestNPCBase::SaveNPCQuest()
{
	//GI->QuestSave->SaveNPC(NPCName, NPCQuests, bQuestSucceed, bIsQuesting, bCanAccept, bHaveMainQuest, CanQuestCnt, CanMainQuestCnt);
	GI->SetNPCSaveSlot(this);
	/* NPC �̸�, NPC ����Ʈ ���, ����Ʈ �Ϸ����, ����Ʈ ������ ����, ����Ʈ�� �����Ҽ� �ִ��� ���� */
}


void AQuestNPCBase::SetIconWidget()
{
	switch (bQuestSucceed) // ����Ʈ �Ϸ� ǥ���ΰ�?
	{
	case true:
		SucceedWidget->SetVisibility(true);
		break;
	case false:
		if (bHaveMainQuest)
		{
			MainIconWidget->SetVisibility(true);
			break;
		}
		else
		{
			IconWidget->SetVisibility(true);
			break;
		}
	
	}
}

void AQuestNPCBase::HiddenIcon()
{
	SucceedWidget->SetVisibility(false);
	IconWidget->SetVisibility(false);
	MainIconWidget->SetVisibility(false);
}


/* ĳ���Ͱ� �ش� NPC�� ����Ʈ�� ���������� �������� �˻��Ѵ�. */
bool AQuestNPCBase::FindCanQuest()
{
	// ���� �ν��Ͻ� ��ȸ�� NPC �̸����� �ϸ� �ɵ��ϴ�.
	/*���� �ν��Ͻ� ���� ������ ĳ���Ͱ� ���డ���� ����Ʈ ����� ����Ʈ �̸����� �����ϰ�
		���⼭ �� ����� ��ȸ�� �ش� NPC�� ���డ���� ����Ʈ�� ������ ������ �ش� ����Ʈ�� 
		bCanAccepted�� true�ϰ� NPC Ŭ������ bCanAccept�� true�� �ٲ۴�.
		�ش� NPC Ŭ������ bCanAccept�� ĳ���Ͱ� ��ó�� ���� �ÿ� ������ ���� �뵵�� ����,
		�� NPCQuest�� bCanAccepted�� ���̾� �α� �Ŀ� ������ ����Ʈ ����� �ҋ� ���.
		
		���⼭ ������ Quest�� ã�������� CanQuestCnt�� �÷��ش�. �ش� ������ ����Ʈ ������ �� ����� ����
		���� �Ѵ�.*/

	if (GI)
	{
		FCanQuestNums* CanQuestNum = GI->PlayerCanQuest.Find(NPCName);
		if (CanQuestNum)
		{
			for (int32 Nums : CanQuestNum->QuestNums)
			{
				for (FNPCQuest& Quests : NPCQuests.Quests)
				{
					if (Quests.QuestNumber == Nums)
					{
						Quests.bCanAccepted = true;
						CanQuestCnt++;

						if (Quests.QuestType == EQuestType::Main)
						{
							CanMainQuestCnt++;
							// ��������Ʈ�� �����Ƿ� ��������Ʈ �������� ���.
						}
					}
				}
			}
			if (CanQuestCnt > 0)
			{
				bCanAccept = true;
				if (CanMainQuestCnt > 0)
				{
					bHaveMainQuest = true;
				}
				return true;
			}
			
		}
	}
	return false;
}


// ������ ����� ���� ����. �ڡ�
bool AQuestNPCBase::CanVisibleWidget()
{
	if (bCanAccept || bQuestSucceed) // �����Ҽ� �ִ°� �ְų�, �Ϸ��Ѱ� �ְų�.
		return true;
	else
		return false;
}