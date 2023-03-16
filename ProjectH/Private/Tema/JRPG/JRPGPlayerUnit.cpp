// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGPlayerUnit.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGPlayerController.h"


void AJRPGPlayerUnit::BeginPlay()
{
	Super::BeginPlay();

}


void AJRPGPlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJRPGPlayerUnit::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AJRPGPlayerUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AJRPGPlayerUnit::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// 이 함수는 아직 위젯을 보이게 하기전에 먼저 위젯 아이콘을 미리 셋팅해두는 함수.
void AJRPGPlayerUnit::BattleTurnStart()
{
	if (OwnerController)
		OwnerController->BattleTurnStart(true);
	// UI에 모든 정보를 초기화 해두고, UI에서 실행.
}


void AJRPGPlayerUnit::UnitBattleStart()
{
	Super::UnitBattleStart();

	if (!OwnerController)
		return;

	OwnerController->CameraSetUp(GetActorLocation());
	OwnerController->EnermyListSetup();
	OwnerController->SetVisibleBattleWidget(true); // 위젯 보이기
	if (bCC) // CC기 상태인 경우 턴 종료.
	{
		OwnerController->SetEnermyTurnWidget(true);
		UnitTurnEndCCState();		
	}		
	else
		OwnerController->SetEnermyTurnWidget(false); // 적 차례시 보이면 안되었던 스킬 버튼 및 적 선택 리스트 보이게 하기.
		

	if (!GM->bBattleTutorial) // 튜토리얼 실행
		OwnerController->BattleTutorialStart();
}


void AJRPGPlayerUnit::DeadBattleListRemove()
{
	TArray<AJRPGUnit*>& OwnerList = GM->OwnerList;
	for (int32 i = 0; i < OwnerList.Num(); i++)
	{
		if (OwnerList[i] == this)
		{
			OwnerList.RemoveAt(i);
			break;
		}
	}
}