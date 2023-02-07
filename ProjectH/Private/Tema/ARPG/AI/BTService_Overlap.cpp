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
	FCollisionQueryParams CollisionParam(NAME_None, false, OwnerPawn); // �ݸ��� �±� , bool bTraceComplex  (���⼺ ���� : �������� ������ �ƴϸ� false ��õ) , ������ ��

	bool bResult = World->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel12, FCollisionShape::MakeSphere(CollisionRadius), CollisionParam);
	
	if (bResult)
	{
		for (auto Overlap : OverlapResults)
		{
			// ���⼭ ��Ƽ�÷��� �����̶�� �� Distance�� ���ؼ� ����� �÷��̾ 
			// �����ؼ� �ش� ���͸� ������ �ڿ� for �ۿ��� Ÿ�����ϸ� �ɵ�.
			// ������ �̱� �÷��� �����̴� ��������.

			//�� ������ ĳ���Ͱ� �ϳ��� �׳� AARPGUnit���� ������ ĳ���Ͱ� ���� ����� ĳ���� ���� Ŭ������ �ϳ� �� �����θ� ���ҵ� �ϴ�.
			AARPGUnit* Unit = Cast<AARPGUnit>(Overlap.GetActor());
			if (Unit)
			{
				// Ʈ���̽��� �� Ž���ϱ� �ڡ�
				FVector StartLocation = OwnerPawn->GetPawnViewLocation();
				FVector EndLocation = Unit->GetActorLocation();
				FHitResult HitResult;

				bool bTrace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);

				if (!bTrace)
				{
					// ������ ������ ���� �̴�.
					if (!OwnerPawn->PlayerUnit) // ���� ���ݴ������ʾ����Ƿ� Ž��.
					{
						if (OwnerPawn->TargetDotProduct(Unit->GetActorLocation(), 0.34)) // 70�� ����
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), Unit);
							//DrawDebugSphere(World, Center, CollisionRadius, 16, FColor::Green, false, 0.2f);

							OwnerPawn->PlayerUnit = Unit; // Ÿ�� �÷��̾� ����.
							OwnerPawn->bMoving = true; // ���� �����
							OwnerPawn->SetBattleMode(true); // ��Ʋ���
							OwnerPawn->SetCollisionRadius(true);
							return;
						}
					}
					else // ���ݴ��ؼ� Ž��
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), Unit);
						//DrawDebugSphere(World, Center, CollisionRadius, 16, FColor::Green, false, 0.2f);

						OwnerPawn->PlayerUnit = Unit; // Ÿ�� �÷��̾� ����.
						OwnerPawn->bMoving = true; // ���� �����
						OwnerPawn->SetBattleMode(true); // ��Ʋ���
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
		OwnerPawn->SetBattleMode(false); // ��Ʋ���
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetUnit"), nullptr);
	//DrawDebugSphere(World, Center, CollisionRadius, 16, FColor::Red, false, 0.2f);
}


