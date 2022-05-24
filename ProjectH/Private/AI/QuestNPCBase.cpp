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
			/* false 면 로드 할게 없음. true면 데이터가 있음.*/
			FindCanQuest(); // 게임 인스턴스에서 퀘스트 목록 검사. 자세한것은 해당 함수에 적어둠.
		}
	}


	if (!NPCQuests.Quests.Num()) // 없으면 그냥 리턴.
		return;

	for (int32 i = 0; i < NPCQuests.Quests.Num(); i++)
	{
		NPCQuests.Quests[i].OwnerNPC = this;
	} //원래는 여기서 트리거의 정보를 저장했다. 월드에 미리 배치된 트리거를 여기서 저장.


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

/* 실시간으로 NPC에 퀘스트 추가하는 방법. 완료 퀘스트도 결국엔 퀘스트 이기때문에 이런식으로 추가 가능.*/
void AQuestNPCBase::AddNPCQuest(FNPCQuest Quest)
{
	//FindCanQuest(); 
	CanQuestCnt++;
	bCanAccept = true;
	//bQuestSucceed = true; 이런식으로 추가와 동시에 Succeed를 하면 바로 완료된걸 수행하게 할수 있을듯하다.
}


void AQuestNPCBase::Interact_Implementation(class AProjectHCharacter* OwnerCharacter)
{
	_DEBUG("HI");
	// 여기서 다이얼 로그를 띄워도 될듯하다.
	// 다이얼 로그 마지막에 버튼으로 가지고 있는 퀘스트를 띄우고 해당퀘스트 버튼을 누르면 QuestInfoOpen함수에 번호를 보내주자.
	// 지금은 없으니 0 보내기.
	QuestInfoOpen(0, Cast<AProjectH_PC>(OwnerCharacter->GetController()));
	// 클릭할수있는 퀘스트가 여러개 일수도있으니 인덱스 번호와 , 컨트롤러 반환.
}


void AQuestNPCBase::QuestInfoOpen(int32 QuestIndex, AProjectH_PC* OwnerController)
{
	//NPCQuests의 Inex 번호로 해당 퀘스트 창을 띄운다.
	//나중에 텍스트를 넘기고 마지막에 퀘스트를 고르는 ui를 구현하기 위해 나눔. (ex.로스트아크)

	if (!OwnerController && !NPCQuests.Quests.IsValidIndex(QuestIndex))
	{
		return;
	}

	OwnerController->MainQuestUI->OpenInfoUI(NPCQuests.Quests[QuestIndex], QuestIndex);
	// 여기서 아예 QuestInfo에 퀘스트 인덱스 및, 퀘스트 이름, 퀘스트 정보를 전달해도 되지만,
	// MainQuest의 Owner 컨트롤러나 퀘스트 컴포넌트도 추가해야하기때문에 Main에서 해결한다.
}

void AQuestNPCBase::SaveNPCQuest()
{
	//GI->QuestSave->SaveNPC(NPCName, NPCQuests, bQuestSucceed, bIsQuesting, bCanAccept, bHaveMainQuest, CanQuestCnt, CanMainQuestCnt);
	GI->SetNPCSaveSlot(this);
	/* NPC 이름, NPC 퀘스트 목록, 퀘스트 완료상태, 퀘스트 중인지 상태, 퀘스트를 수락할수 있는지 상태 */
}


void AQuestNPCBase::SetIconWidget()
{
	switch (bQuestSucceed) // 퀘스트 완료 표시인가?
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


/* 캐릭터가 해당 NPC의 퀘스트를 수행조건이 가능한지 검사한다. */
bool AQuestNPCBase::FindCanQuest()
{
	// 게임 인스턴스 조회는 NPC 이름으로 하면 될듯하다.
	/*게임 인스턴스 같은 곳에서 캐릭터가 수행가능한 퀘스트 목록을 퀘스트 이름으로 저장하고
		여기서 그 목록을 조회해 해당 NPC가 수행가능한 퀘스트를 가지고 있으면 해당 퀘스트의 
		bCanAccepted를 true하고 NPC 클래스의 bCanAccept를 true로 바꾼다.
		해당 NPC 클래스의 bCanAccept는 캐릭터가 근처에 왔을 시에 위젯을 띄우기 용도로 쓰고,
		★ NPCQuest의 bCanAccepted는 다이얼 로그 후에 나열할 퀘스트 목록을 할떄 사용.
		
		여기서 가능한 Quest를 찾을때마다 CanQuestCnt도 늘려준다. 해당 값으로 퀘스트 위젯을 더 띄울지 말지
		결정 한다.*/

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
							// 메인퀘스트가 있으므로 메인퀘스트 아이콘을 출력.
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


// 위젯을 띄울지 말지 결정. ★★
bool AQuestNPCBase::CanVisibleWidget()
{
	if (bCanAccept || bQuestSucceed) // 수락할수 있는게 있거나, 완료한게 있거나.
		return true;
	else
		return false;
}