// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BT/JRPG_FindTargetService.h"
#include "Tema/JRPG/JRPGEnermy.h"
#include "Tema/JRPG/JRPGFieldAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"


UJRPG_FindTargetService::UJRPG_FindTargetService()
{
	NodeName = TEXT("JRPGFindTarget");
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel8));
}


void UJRPG_FindTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AJRPGEnermy* OwnerEnermy = Cast<AJRPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerEnermy)
		return;

	UWorld* World = OwnerEnermy->GetWorld();
	if (!World)
		return;

	TArray<AActor*> OutActors;
	bool bFind = UKismetSystemLibrary::SphereOverlapActors(World, OwnerEnermy->GetActorLocation(), 1000.f, ObjectTypes, nullptr, IgnoreActors, OutActors);
	if (bFind)
	{
		for (AActor* Actor : OutActors)
		{
			_DEBUG("Find");
			class AJRPGUnit* TargetUnit = Cast<class AJRPGUnit>(Actor);
			if (TargetUnit)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), TargetUnit);
				return;
			}
		}

	}

	return OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), nullptr);

}
