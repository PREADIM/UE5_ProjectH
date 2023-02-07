// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/BTService_Overlap.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Kismet/KismetSystemLibrary.h"

UBTService_Overlap::UBTService_Overlap()
{
	NodeName = TEXT("ARPGOverlap");
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
}

void UBTService_Overlap::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	/*AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawn)
	{
		_DEBUG("OwnerPawn Fail");
		return;
	}*/

	AARPGEnermy* OwnerPawn = Cast<AARPGEnermy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("CurrentUnit")));
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
	
	if (bResult)
	{
		for (auto Overlap : OverlapResults)
		{
			// 여기서 멀티플레이 게임이라면 각 Distance를 구해서 가까운 플레이어를 
			// 지정해서 해당 액터를 가져온 뒤에 for 밖에서 타겟팅하면 될듯.
			// 지금은 싱글 플레이 게임이니 하지않음.

			//이 게임은 캐릭터가 하나라서 그냥 AARPGUnit으로 했지만 캐릭터가 많은 경우라면 캐릭터 기초 클래스를 하나 더 만들어두면 편할듯 하다.
			AARPGUnit* Unit = Cast<AARPGUnit>(Overlap.GetActor());
			if (Unit)
			{
				// 트레이스로 벽 탐지하기 ★★
				FVector StartLocation = OwnerPawn->GetPawnViewLocation();
				FVector EndLocation = Unit->GetActorLocation();
				FHitResult HitResult;

				bool bTrace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);

				if (!bTrace)
				{
					// 보스는 무조건 추적 이다.
					if (!OwnerPawn->PlayerUnit) // 아직 공격당하지않았으므로 탐지.
					{
						if (OwnerPawn->TargetDotProduct(Unit->GetActorLocation(), 0.34)) // 70도 가량
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), Unit);
							//DrawDebugSphere(World, Center, CollisionRadius, 16, FColor::Green, false, 0.2f);

							OwnerPawn->PlayerUnit = Unit; // 타겟 플레이어 설정.
							OwnerPawn->bMoving = true; // 무빙 실행★
							OwnerPawn->SetBattleMode(true); // 배틀모드
							OwnerPawn->SetCollisionRadius(true);
							return;
						}
					}
					else // 공격당해서 탐지
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), Unit);
						//DrawDebugSphere(World, Center, CollisionRadius, 16, FColor::Green, false, 0.2f);

						OwnerPawn->PlayerUnit = Unit; // 타겟 플레이어 설정.
						OwnerPawn->bMoving = true; // 무빙 실행★
						OwnerPawn->SetBattleMode(true); // 배틀모드
						OwnerPawn->SetCollisionRadius(true);
						return;
					}
				}
				
			}
		}	
	}
	else
	{
		OwnerPawn->PlayerUnit = nullptr;
		OwnerPawn->SetBattleMode(false); // 배틀모드
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), nullptr);
	//DrawDebugSphere(World, Center, CollisionRadius, 16, FColor::Red, false, 0.2f);
}


