// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGGameMode.h"




AJRPGAIController::AJRPGAIController()
{
	BB = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));
}

void AJRPGAIController::BeginPlay()
{
	Super::BeginPlay();
}


void AJRPGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 만일 멀티플레이 게임이라면, 사이퍼즈처럼 상대의 유닛을 AI가 조종하게 하려면 여기서 ActorTag를 다 지우고 Enermy라는 태그를 붙이고
	// 이를 기반하여 BT에서 제작한다면 상대 플레이어를 찾을때에는 Tags를 기반하여 플레이어를 찾게하는 방법이 가능 하지 않을까? 하는 추축. 
	// 팀전인 경우는 애초에 Red, Blue 태그를 하면 될듯

	OwnerCharacter = Cast<AJRPGUnit>(InPawn);
	if (OwnerCharacter)
	{
		AJRPGGameMode* GM = Cast<AJRPGGameMode>(GetWorld()->GetAuthGameMode());
		if (!GM)
			return;

		if (OwnerCharacter->GetBT())
		{
			if (!BB->InitializeBlackboard(*(OwnerCharacter->GetBT()->BlackboardAsset))) // BT에 있는 블랙보드를 적용
				_DEBUG("InitializeBlackboard ERROR");
			BB->SetValueAsObject(FName("GM"), GM);
			RunBehaviorTree(OwnerCharacter->GetBT()); // 캐릭터마다 무브셋이 다른것도 이런 방식으로 캐릭터 고유의 BT로 처리가능	
		}

		OwnerCharacter->BattleAIController = this;
		SetIsTurn(false);
	}
}


void AJRPGAIController::OnUnPossess()
{
	Super::OnUnPossess();
}


void AJRPGAIController::SetIsTurn(bool bFlag)
{
	if (BB->GetBlackboardAsset() != nullptr)
		BB->SetValueAsBool(FName("IsTurn"), bFlag);
}

