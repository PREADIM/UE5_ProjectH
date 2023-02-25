// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/QuestComponent/TriggerEventBase.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "UI/QuestIcon.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/ProjectHCharacter.h"
#include "GameMode/ProjectHGameInstance.h"
#include "AI/QuestNPCBase.h"

// Sets default values
ATriggerEventBase::ATriggerEventBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<class USceneComponent>(TEXT("Root"));
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

	RootComponent = Root;
	Widget->SetupAttachment(Root);

	static ConstructorHelpers::FClassFinder<UQuestIcon> BP_SucceedIcon(TEXT("WidgetBlueprint'/Game/PROJECT/BP_CLASS/Blueprints/04_Special/BP_QuestSystem/BP_SucceedIcon'"));
	if (BP_SucceedIcon.Succeeded())
	{
		Widget->SetWidgetClass(BP_SucceedIcon.Class);
		Widget->SetDrawSize(FVector2D(100.f, 130.f));
	}

	Widget->SetVisibility(false);
	SetActorTickEnabled(false);	
}

// Called when the game starts or when spawned
void ATriggerEventBase::BeginPlay()
{
	Super::BeginPlay();
	Widget->InitWidget();
}

// Called every frame
void ATriggerEventBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter != nullptr)
	{
		UQuestIcon* Icon = Cast<UQuestIcon>(Widget->GetUserWidgetObject());
		if (Icon)
		{
			int32 Dist = UKismetMathLibrary::FTrunc(GetDistanceTo(PlayerCharacter) / 100);
			if (Icon->Distance != Dist)
			{
				Icon->Init(Dist);
			}
		}
	}
}


/*void ATriggerEventBase::SetInit()
{
	트리거가 NPC를 소환해서 해당 NPC에게 퀘스트를 준다거나,
	트리거가 무언가 해야하는 일이 있을때 사용할 함수.
	게임실행시 QuestComponent의 AddQuest에서 실행시킨다. 블루프린트에서 작성하도록 만든다.
}*/


void ATriggerEventBase::TriggerDestroy()
{
	TriggerDestroyBPBind();
	Destroy();
}

void ATriggerEventBase::SetTriggerWidget()
{
	if (QuestComponent)
	{
		if (QuestComponent->GetActiveQuest().QuestSteps[0].Trigger == this)
		{
			Widget->SetVisibility(true);
			SetActorTickEnabled(true);
		}
	}

}

void ATriggerEventBase::SetHiddenTriggerWidget()
{
	if (QuestComponent)
	{
		if (QuestComponent->GetActiveQuest().QuestSteps[0].Trigger == this)
		{
			Widget->SetVisibility(false);
			SetActorTickEnabled(false);
		}
	}
}



bool ATriggerEventBase::IsThisTrigger()
{
	if (QuestComponent != nullptr)
	{
		if (QuestComponent->GetActiveQuest().QuestSteps.IsEmpty())
			return false;

		// 액티브 퀘스트에 맞춰서 실행하는 조건.
		if (QuestComponent->GetActiveQuest().QuestSteps[0].Trigger == this) // 같은 가?
		{
			SetHiddenTriggerWidget(); // 틱과 위젯 끄기. 원신의 퀘스트 트리거를 생각해보자.
			SetInit();
			return true;
		}
	}
	return false;
}

void ATriggerEventBase::QuestNextStep()
{
	QuestComponent->CompleteStep();
}


/* 진행률이 있는 퀘스트의 진행률의 설명을 다시 바인드. */
void ATriggerEventBase::BindProgressCnt()
{
	if (QuestComponent)
		QuestComponent->BindSetDescription();

}

// 퀘스트를 완료한게아니라 완료 가능해졌다는 뜻.
void ATriggerEventBase::ClearQuest()
{
	if (GI && PlayerCharacter)
	{
		AQuestNPCBase* NPC = GI->GetNPCPtr(OwnerNPCName);
		if (NPC)
		{
			TriggerDestroy();
			NPC->SucceedQuestsNums.Emplace(QuestNumber);
		}
		PlayerCharacter->QuestCollisionSetUp();
	}
}
