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
	FCollisionQueryParams CollisionParam(NAME_None, false, OwnerPawn); // �ݸ��� �±� , bool bTraceComplex  (���⼺ ���� : �������� ������ �ƴϸ� false ��õ) , ������ ��

	bool bResult = World->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel12, FCollisionShape::MakeSphere(CollisionRadius), CollisionParam);

	// ������ �������� �������ؼ� �׽�Ʈ������, ���������Ͽ� �ü� ������ �����Ͽ� �� ���ΰ�� ���� Ÿ�����ϴ� ����� ���Ѵ�.

	if (bResult)
	{
		for (auto Overlap : OverlapResults)
		{
			// ���⼭ ��Ƽ�÷��� �����̶�� �� Distance�� ���ؼ� ����� �÷��̾ 
			// �����ؼ� �ش� ���͸� ������ �ڿ� for �ۿ��� Ÿ�����ϸ� �ɵ�.
			// ������ �̱� �÷��� �����̴� ��������.
			
			AARPGUnit* Unit = Cast<AARPGUnit>(Overlap.GetActor());
			if (Unit)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), Unit);
				DrawDebugSphere(World, Center, CollisionRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, Unit->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
				//_DEBUG("%s", *OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetUnit"))->GetName());

				OwnerPawn->PlayerUnit = Unit; // Ÿ�� �÷��̾� ����.
				if(OwnerPawn->bHitting == false)
					OwnerPawn->bMoving = true; // ���� �����
				OwnerPawn->SetBattleMode(true); // ��Ʋ���
				OwnerPawn->SetCollisionRadius(true);
				return;
			}
		}	
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), nullptr);
	DrawDebugSphere(World, Center, CollisionRadius, 16, FColor::Red, false, 0.2f);
}


