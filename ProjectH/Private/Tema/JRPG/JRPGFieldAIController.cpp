// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGFieldAIController.h"
#include "Tema/JRPG/JRPGEnermy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


AJRPGFieldAIController::AJRPGFieldAIController()
{
	BB = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BB"));
}


void AJRPGFieldAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	FieldUnit = Cast<AJRPGFieldEnermy>(InPawn);
	if (FieldUnit)
	{
		FieldUnit->OwnerAIController = this;
		BB->InitializeBlackboard(*(FieldUnit->GetBT()->BlackboardAsset));
		RunBehaviorTree(FieldUnit->GetBT());
	}
}


void AJRPGFieldAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AJRPGFieldAIController::BattleStart()
{
	BrainComponent->StopLogic(FString(""));
}

void AJRPGFieldAIController::ReturnToField()
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AJRPGFieldAIController::RestartBT, 1.5f, false);
}

void AJRPGFieldAIController::RestartBT()
{
	BrainComponent->RestartLogic();
}
