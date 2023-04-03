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
	// UI�� ��� ������ �ʱ�ȭ �صΰ�, UI���� ����.
}

void AJRPGEnermyUnit::UnitBattleStart()
{
	Super::UnitBattleStart();

	if (!OwnerController)
		return;

	OwnerController->SetVisibleBattleWidget(true); // ���� ���̱�
	OwnerController->SkillAndListButtonHidden(true); // ���� ���ʴϱ� ������ �ʿ��Ѱ͸� �����.

	if (!BattleAIController)
		return;

	if (bCC) // CC�� ������ ��� ��ŵ
	{
		OwnerController->TargetUnit = GM->OwnerList[0];
		OwnerController->EnermyUnitTurnToTarget();
		UnitTurnEndCCState();
	}	
	else
		BattleAIController->SetIsTurn(true); // ���̴�.

	// ���� ���� �ൿ ����.
	// ���� ���� �� ĳ���� Ÿ�� ����.
	// Ÿ���� �������� �ش� Ÿ������ ī�޶� �̵�. 
	// (�ش� ĳ���Ͱ� �� ���� �����־���ϹǷ�, �� ���� ���ؼ� ī�޶� ȸ���ؾ��Ѵ�.)
	// (FindLookAtRotation) �� Ȱ���ϸ� �ɵ�.
	// �� �ൿ�� ����.
	// BT�� ����
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
