// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTService_Overlap.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/ARPG/ARPGEnermy.h"

UBTService_Overlap::UBTService_Overlap()
{
	NodeName = TEXT("ARPGOverlap");
}

void UBTService_Overlap::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn Fail");
		return;
	}

	UWorld* World = OwnerPawn->GetWorld();
	FVector Center = OwnerPawn->GetActorLocation();
	float CollisionRadius = OwnerPawn->GetCollisionRadius();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParam(NAME_None, false, OwnerPawn); // 콜리전 태그 , bool bTraceComplex  (복잡성 여부 : 디테일한 판정이 아니면 false 추천) , 무시할 폰

	bool bResult = World->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel12, FCollisionShape::MakeSphere(CollisionRadius), CollisionParam);

	// 지금은 원형으로 오버랩해서 테스트하지만, 내적값을하여 시선 각도를 설정하여 그 값인경우 적이 타겟팅하는 방식을 취한다.

	if (bResult)
	{
		for (auto Overlap : OverlapResults)
		{
			// 여기서 멀티플레이 게임이라면 각 Distance를 구해서 가까운 플레이어를 
			// 지정해서 해당 액터를 가져온 뒤에 for 밖에서 타겟팅하면 될듯.
			// 지금은 싱글 플레이 게임이니 하지않음.
			
			AARPGUnit* Unit = Cast<AARPGUnit>(Overlap.GetActor());
			if (Unit)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), Unit);
				DrawDebugSphere(World, Center, CollisionRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, Unit->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
				//_DEBUG("%s", *OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetUnit"))->GetName());

				OwnerPawn->PlayerUnit = Unit; // 타겟 플레이어 설정.
				if(OwnerPawn->bHitting == false)
					OwnerPawn->bMoving = true; // 무빙 실행★
				OwnerPawn->SetBattleMode(true); // 배틀모드
				OwnerPawn->SetCollisionRadius(true);
				return;
			}
		}	
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), nullptr);
	DrawDebugSphere(World, Center, CollisionRadius, 16, FColor::Red, false, 0.2f);
}


