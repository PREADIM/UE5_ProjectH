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
#include "UI/QuestIcon.h"
#include "Components/CanvasPanelSlot.h"

// Sets default values
ATriggerEventBase::ATriggerEventBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<class USceneComponent>(TEXT("Root"));
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Root;
	Collision->SetupAttachment(Root);
	SetActorTickEnabled(false);		
}

void ATriggerEventBase::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ATriggerEventBase::OverlapTrigger);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ATriggerEventBase::OverlapEndTrigger);

	if (BP_QuestIcon && PlayerController)
	{
		QuestIcon = CreateWidget<UQuestIcon>(GetWorld(), BP_QuestIcon);
		if (QuestIcon)
		{
			QuestIcon->SetRenderOpacity(0.f);
			IconCanvasSlot = PlayerController->AddChildCanvas(QuestIcon);
		}		
	}
	SetupMainIconWidget();
	SetupCollision();
}


void ATriggerEventBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetupMainIconWidget();
}

void ATriggerEventBase::SetupMainIconWidget()
{
	if (PlayerCharacter)
	{
		if (QuestIcon)
		{
			int32 Dist = UKismetMathLibrary::FTrunc(GetDistanceTo(PlayerCharacter) / 100);
			if (QuestIcon->Distance != Dist)
				QuestIcon->Init(Dist);

			if (PlayerController)
				PlayerController->MainQuestIconWidgetSetup(IconCanvasSlot, GetActorLocation());				
		}
	}
}

void ATriggerEventBase::OverlapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == PlayerCharacter)
		IsThisTrigger();
}

void ATriggerEventBase::OverlapEndTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == PlayerCharacter)
		SetTriggerWidget();
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
	QuestIcon->RemoveFromParent();
	Destroy();
}

void ATriggerEventBase::SetTriggerWidget()
{
	if (QuestComponent)
	{
		if (QuestComponent->GetActiveQuest().QuestNumber == QuestNumber)
		{
			if (bBPBindVisibleTriggerFunc)
				SetVisibleTriggerWidgetBind();

			if (QuestIcon)
				QuestIcon->SetRenderOpacity(1.f);		
			else
			{
				if (PlayerController)
				{
					QuestIcon = CreateWidget<UQuestIcon>(GetWorld(), BP_QuestIcon);
					if (QuestIcon)
						IconCanvasSlot = PlayerController->AddChildCanvas(QuestIcon);
						
				}
			}
			SetActorTickEnabled(true);
			SetupCollision();
		}
	}

}

void ATriggerEventBase::SetHiddenTriggerWidget()
{
	if (QuestComponent)
	{
		if (QuestComponent->GetActiveQuest().QuestNumber == QuestNumber)
		{
			if (bBPBindHiddenTriggerFunc) // BP에서 무언가 더 할일이 있다.
				SetHiddenTriggerWidgetBind();

			if (QuestIcon)
				QuestIcon->SetRenderOpacity(0.f);
			
			SetActorTickEnabled(false);
		}
	}
}



bool ATriggerEventBase::IsThisTrigger()
{
	if (QuestComponent)
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
