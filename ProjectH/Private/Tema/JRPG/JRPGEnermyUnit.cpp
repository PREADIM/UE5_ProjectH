// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGEnermyUnit.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGAIController.h"


void AJRPGEnermyUnit::BeginPlay()
{
	Super::BeginPlay();

}


void AJRPGEnermyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJRPGEnermyUnit::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AJRPGEnermyUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AJRPGEnermyUnit::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AJRPGEnermyUnit::BattleTurnStart()
{
	if (OwnerController)
		OwnerController->BattleTurnStart(false);
	// UI에 모든 정보를 초기화 해두고, UI에서 실행.
}

void AJRPGEnermyUnit::UnitBattleStart()
{
	Super::UnitBattleStart();

	if (!OwnerController)
		return;

	OwnerController->SetVisibleBattleWidget(true); // 위젯 보이기
	OwnerController->SkillAndListButtonHidden(true); // 적의 차례니까 위젯을 필요한것만 남긴다.

	if (!BattleAIController)
		return;

	if (bCC) // CC기 상태인 경우 스킵
	{
		OwnerController->TargetUnit = GM->OwnerList[0];
		OwnerController->EnermyUnitTurnToTarget();
		UnitTurnEndCCState();
	}	
	else
		BattleAIController->SetIsTurn(true); // 턴이다.

	// 적이 취할 행동 설정.
	// 적이 때릴 내 캐릭터 타겟 설정.
	// 타겟을 정했으면 해당 타겟으로 카메라 이동. 
	// (해당 캐릭터가 그 적을 보고있어야하므로, 그 적을 향해서 카메라가 회전해야한다.)
	// (FindLookAtRotation) 을 활용하면 될듯.
	// 적 행동을 실행.
	// BT로 구현
}



void AJRPGEnermyUnit::DeadBattleListRemove()
{
	TArray<AJRPGUnit*>& EnermyList = GM->EnermyList;
	for (int32 i = 0; i < EnermyList.Num(); i++)
	{
		if (EnermyList[i] == this)
		{
			EnermyList.RemoveAt(i);
			break;
		}		
	}
}
