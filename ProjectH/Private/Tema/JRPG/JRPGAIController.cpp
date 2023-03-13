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
	if (OwnerCharacter)
	{
		AJRPGGameMode* GM = Cast<AJRPGGameMode>(GetWorld()->GetAuthGameMode());
		if (!GM)
			return;

		if (OwnerCharacter->GetBT())
		{
			if (!BB->InitializeBlackboard(*(OwnerCharacter->GetBT()->BlackboardAsset))) // BT�� �ִ� �����带 ����
				_DEBUG("InitializeBlackboard ERROR");
			BB->SetValueAsObject(FName("GM"), GM);
			RunBehaviorTree(OwnerCharacter->GetBT()); // ĳ���͸��� ������� �ٸ��͵� �̷� ������� ĳ���� ������ BT�� ó������	
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

