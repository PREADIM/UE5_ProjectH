// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/QuestComponent/TriggerEventBase.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "UI/QuestIcon.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/ProjectHCharacter.h"

// Sets default values
ATriggerEventBase::ATriggerEventBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

	bIsQuesting = false;
	Widget->SetVisibility(false);
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
		FVector V = GetActorLocation() - PlayerCharacter->GetActorLocation();
		float Range = UKismetMathLibrary::MapRangeUnclamped(V.Length(), 100, 5000, 1.0, 0.7);

		UQuestIcon* Icon = Cast<UQuestIcon>(Widget->GetUserWidgetObject());
		if (Icon)
		{
			Icon->SetRenderScale(FVector2D(Range, Range));
			int32 Dist = UKismetMathLibrary::FTrunc(PlayerCharacter->GetDistanceTo(this) / 100);
			if (Icon->Distance != Dist)
			{
				Icon->Distance = Dist;
				Icon->Init();
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


/* 이 함수가 호출되면 저장되어있는 QuestComponent를 이용해서 퀘스트를 다음 스텝으로 넘겨준다.*/
void ATriggerEventBase::QuestClear()
{
	if (QuestComponent != nullptr)
	{
		if (QuestComponent->GetActiveQuest().bSucceed) // 퀘스트 조건을 채웠는가?
			IsThisTrigger();
	}
}


bool ATriggerEventBase::IsThisTrigger()
{
	if (QuestComponent != nullptr)
	{
		if (QuestComponent->GetActiveQuest().QuestSteps.IsEmpty())
			return false;

		if (QuestComponent->GetActiveQuest().QuestSteps[0].Trigger == this) // 같은 가?
		{
			QuestNextStep();
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