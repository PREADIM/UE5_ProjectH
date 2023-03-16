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

	AJRPGFieldEnermy* OwnerEnermy = Cast<AJRPGFieldEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerEnermy)
		return;

	UWorld* World = OwnerEnermy->GetWorld();
	if (!World)
		return;

	TArray<AActor*> OutActors;
	bool bFind = UKismetSystemLibrary::SphereOverlapActors(World, OwnerEnermy->GetActorLocation(), 700.f, ObjectTypes, nullptr, IgnoreActors, OutActors);
	if (bFind)
	{
		for (AActor* Actor : OutActors)
		{
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
