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
	Ʈ���Ű� NPC�� ��ȯ�ؼ� �ش� NPC���� ����Ʈ�� �شٰų�,
	Ʈ���Ű� ���� �ؾ��ϴ� ���� ������ ����� �Լ�.
	���ӽ���� QuestComponent�� AddQuest���� �����Ų��. �������Ʈ���� �ۼ��ϵ��� �����.
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


/* �� �Լ��� ȣ��Ǹ� ����Ǿ��ִ� QuestComponent�� �̿��ؼ� ����Ʈ�� ���� �������� �Ѱ��ش�.*/
void ATriggerEventBase::QuestClear()
{
	if (QuestComponent != nullptr)
	{
		if (QuestComponent->GetActiveQuest().bSucceed) // ����Ʈ ������ ä���°�?
			IsThisTrigger();
	}
}


bool ATriggerEventBase::IsThisTrigger()
{
	if (QuestComponent != nullptr)
	{
		if (QuestComponent->GetActiveQuest().QuestSteps.IsEmpty())
			return false;

		if (QuestComponent->GetActiveQuest().QuestSteps[0].Trigger == this) // ���� ��?
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


/* ������� �ִ� ����Ʈ�� ������� ������ �ٽ� ���ε�. */
void ATriggerEventBase::BindProgressCnt()
{
	if (QuestComponent)
		QuestComponent->BindSetDescription();

}