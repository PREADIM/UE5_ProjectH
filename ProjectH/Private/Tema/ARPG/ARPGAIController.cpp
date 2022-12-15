// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGAIController.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AARPGAIController::AARPGAIController()
{
	BB = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));
}

void AARPGAIController::BeginPlay()
{
	Super::BeginPlay();

}


void AARPGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerUnit = Cast<AARPGEnermy>(InPawn);
	if (OwnerUnit)
	{
		if (!BB->InitializeBlackboard(*(OwnerUnit->GetBT()->BlackboardAsset)))
		{
			_DEBUG("Not Init BB");
		}

		OwnerUnit->OwnerAIController = this;

		RunBehaviorTree(OwnerUnit->GetBT());
		BB->SetValueAsVector(FName("DefaultLocation"), OwnerUnit->GetActorLocation());
		BB->SetValueAsObject(FName("CurrentUnit"), OwnerUnit);
	}

}


void AARPGAIController::OnUnPossess()
{

}