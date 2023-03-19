// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/QuestComponent/TriggerEventBase.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "UI/QuestIcon.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/ProjectHCharacter.h"
#include "GameMode/ProjectHGameInstance.h"
#include "AI/QuestNPCBase.h"
#include "Controller/ProjectH_PC.h"

// Sets default values
ATriggerEventBase::ATriggerEventBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<class USceneComponent>(TEXT("Root"));
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

	RootComponent = Root;
	Widget->SetupAttachment(Root);
	Collision->SetupAttachment(Root);

	static ConstructorHelpers::FClassFinder<UQuestIcon> BP_SucceedIcon(TEXT("WidgetBlueprint'/Game/PROJECT/BP_CLASS/Blueprints/04_Special/BP_QuestSystem/BP_SucceedIcon'"));
	if (BP_SucceedIcon.Succeeded())
	{
		Widget->SetWidgetClass(BP_SucceedIcon.Class);
		Widget->SetDrawSize(FVector2D(100.f, 100.f));
	}

	Widget->SetVisibility(false);
	SetActorTickEnabled(false);	

	
}

// Called when the game starts or when spawned
void ATriggerEventBase::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ATriggerEventBase::OverlapTrigger);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ATriggerEventBase::OverlapEndTrigger);

	if (PlayerController)
	{
		PlayerController->OnVisibleWidget.AddUFunction(this, FName("VisibleWidget"));
		PlayerController->OnHiddenWidget.AddUFunction(this, FName("HiddenWidget"));
		_DEBUG("SetupBind");
	}

	Widget->InitWidget();
	SetupCollision();
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


void ATriggerEventBase::OverlapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == PlayerCharacter)
	{
		IsThisTrigger();
	}
}

void ATriggerEventBase::OverlapEndTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == PlayerCharacter)
	{
		_DEBUG("EndTrigger");
		SetTriggerWidget();
	}

}

void ATriggerEventBase::SetupCollision()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}), 0.1, false);
}

void ATriggerEventBase::TriggerDestroy()
{
	TriggerDestroyBPBind();
	Destroy();
}

void ATriggerEventBase::SetTriggerWidget()
{
	if (QuestComponent != nullptr)
	{
		if (QuestComponent->GetActiveQuest().QuestNumber == QuestNumber)
		{
			if (bBPBindVisibleTriggerFunc)
				SetVisibleTriggerWidgetBind();

			Widget->SetVisibility(true);
			SetActorTickEnabled(true);
			SetupCollision();
		}
	}

}

void ATriggerEventBase::SetHiddenTriggerWidget()
{
	if (QuestComponent != nullptr)
	{
		if (QuestComponent->GetActiveQuest().QuestNumber == QuestNumber)
		{
			if (bBPBindHiddenTriggerFunc) // BP에서 무언가 더 할일이 있다.
				SetHiddenTriggerWidgetBind();

			Widget->SetVisibility(false);
			SetActorTickEnabled(false);
		}
	}
}



bool ATriggerEventBase::IsThisTrigger()
{
	if (QuestComponent != nullptr)
	{
		if (QuestComponent->GetHaveQuestNums()->Find(QuestNumber))
		{
			if (bOverlapHiddenWidget)
				SetHiddenTriggerWidget(); // 틱과 위젯 끄기. 원신의 퀘스트 트리거를 생각해보자.
			SetInit();
			return true;
		}
	}
	return false;
}

void ATriggerEventBase::QuestNextStep()
{
	// 가지고 있는 퀘스트라면
	if(QuestComponent->GetHaveQuestNums()->Find(QuestNumber))
		QuestComponent->CompleteStep(QuestNumber);
}


/* 진행률이 있는 퀘스트의 진행률의 설명을 다시 바인드. */
void ATriggerEventBase::BindProgressCnt()
{
	if (QuestComponent)
		QuestComponent->BindSetDescription(QuestNumber);

}



// 퀘스트가 끝나는 시점의 QuestSucceed트리거에서 BeginPlay에서 실행하면 될듯 하다.
// 퀘스트를 완료한게아니라 완료 가능해졌다는 뜻.
void ATriggerEventBase::CanClearQuest()
{
	if (GI && PlayerCharacter)
	{
		AQuestNPCBase* NPC = GI->GetNPCPtr(OwnerNPCName);
		if (NPC)
		{
			SetActorLocation(NPC->GetActorLocation());
			NPC->SucceedQuestsNums.Emplace(QuestNumber);
		}
		PlayerCharacter->QuestCollisionSetUp();
	}
}

void ATriggerEventBase::VisibleWidget()
{
	SetTriggerWidget();
}


void ATriggerEventBase::HiddenWidget()
{
	SetHiddenTriggerWidget();
}
