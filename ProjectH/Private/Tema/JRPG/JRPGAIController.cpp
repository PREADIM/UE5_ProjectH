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

	// ���� ��Ƽ�÷��� �����̶��, ��������ó�� ����� ������ AI�� �����ϰ� �Ϸ��� ���⼭ ActorTag�� �� ����� Enermy��� �±׸� ���̰�
	// �̸� ����Ͽ� BT���� �����Ѵٸ� ��� �÷��̾ ã�������� Tags�� ����Ͽ� �÷��̾ ã���ϴ� ����� ���� ���� ������? �ϴ� ����. 
	// ������ ���� ���ʿ� Red, Blue �±׸� �ϸ� �ɵ�

	OwnerCharacter = Cast<AJRPGUnit>(InPawn);
	if (OwnerCharacter && OwnerCharacter->GetBT())
	{
		if (!BB->InitializeBlackboard(*(OwnerCharacter->GetBT()->BlackboardAsset))) // BT�� �ִ� �����带 ����
			_DEBUG("InitializeBlackboard ERROR");

		AJRPGGameMode* GM = Cast<AJRPGGameMode>(GetWorld()->GetAuthGameMode());
		if (!GM)
		{
			_DEBUG("Not AIController GM");
			return;
		}

		OwnerCharacter->OwnerAIController = this;
		BB->SetValueAsObject(FName("GM"), GM);
	}
	else
	{
		_DEBUG("Blackboard ERROR");
	}

	RunBehaviorTree(OwnerCharacter->GetBT());
	SetIsTurn(false);

}


void AJRPGAIController::OnUnPossess()
{
	Destroy();
}


void AJRPGAIController::SetIsTurn(bool bFlag)
{
	if (BB->GetBlackboardAsset() != nullptr)
	{
		BB->SetValueAsBool(FName("IsTurn"), bFlag);
	}
}