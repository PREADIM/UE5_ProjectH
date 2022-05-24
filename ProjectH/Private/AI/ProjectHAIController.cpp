// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ProjectHAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AI/ProjectHAICharacter.h"


AProjectHAIController::AProjectHAIController()
{
	BB = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));
}

void AProjectHAIController::BeginPlay()
{
	Super::BeginPlay();
	if (OwnerAICharacter)
	{
		RunBehaviorTree(OwnerAICharacter->GetBT());
	}
	

}


void AProjectHAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	OwnerAICharacter = Cast<AProjectHAICharacter>(InPawn);
	if (OwnerAICharacter && OwnerAICharacter->GetBT())
	{
		if (!BB->InitializeBlackboard(*(OwnerAICharacter->GetBT()->BlackboardAsset))) // BT에 있는 블랙보드를 적용
			_DEBUG("InitializeBlackboard ERROR");

	}

}


void AProjectHAIController::OnUnPossess()
{

}