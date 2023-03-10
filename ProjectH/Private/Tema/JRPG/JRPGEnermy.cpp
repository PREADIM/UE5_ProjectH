// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGEnermy.h"
#include "Tema/JRPG/JRPGFieldAIController.h"
#include "Tema/JRPG/JRPGGameMode.h"

// Sets default values
AJRPGEnermy::AJRPGEnermy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.f, 0.f);
	bDead = false;
}

// Called when the game starts or when spawned
void AJRPGEnermy::BeginPlay()
{
	Super::BeginPlay();
	GM = Cast<AJRPGGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		if (GM->GetSaveEnermyUnits(FieldEnermyNumber))
			Destroy(); // ��� �÷��̶� �ִٴ� ���� �̹� ����ó���� �ѹ��̶� ������ �ִ� ���̴� �ٷ� ����.
		else
		{
			DropStruct = GM->GetFieldEnermyDropStruct(FieldEnermyNumber);
		}
	}	
}



void AJRPGEnermy::FieldEnermyDead()
{
	DeadUnit();
	bDead = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GM->SetSaveEnermyUnits(this);
	PlayAnimMontage(DeadMontage);	
}


void AJRPGEnermy::DeadUnit()
{
	if(bBPBindFunc)
		BPBindFunc_DeadUnit();
	//���⿡�� ���� ȣ���� �Լ� ����.
}


void AJRPGEnermy::ReturnToField()
{
	if(OwnerAIController)
		OwnerAIController->ReturnToField();
}


void AJRPGEnermy::BattleStart()
{
	if (OwnerAIController)
		OwnerAIController->BattleStart();
}


class UBehaviorTree* AJRPGEnermy::GetBT()
{
	if (BT)
		return BT;
	else
		return nullptr;
}