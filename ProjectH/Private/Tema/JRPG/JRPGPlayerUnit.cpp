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

// �� �Լ��� ���� ������ ���̰� �ϱ����� ���� ���� �������� �̸� �����صδ� �Լ�.
void AJRPGPlayerUnit::BattleTurnStart()
{
	if (OwnerController)
		OwnerController->BattleTurnStart(true);
	// UI�� ��� ������ �ʱ�ȭ �صΰ�, UI���� ����.
}


void AJRPGPlayerUnit::UnitBattleStart()
{
	Super::UnitBattleStart();

	if (!OwnerController)
		return;

	OwnerController->CameraSetUp(GetActorLocation());
	OwnerController->EnermyListSetup();
	OwnerController->SetVisibleBattleWidget(true); // ���� ���̱�
	if (bCC) // CC�� ������ ��� �� ����.
	{
		OwnerController->SetEnermyTurnWidget(true);
		UnitTurnEndCCState();		
	}		
	else
		OwnerController->SetEnermyTurnWidget(false); // �� ���ʽ� ���̸� �ȵǾ��� ��ų ��ư �� �� ���� ����Ʈ ���̰� �ϱ�.
		

	if (!GM->bBattleTutorial) // Ʃ�丮�� ����
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