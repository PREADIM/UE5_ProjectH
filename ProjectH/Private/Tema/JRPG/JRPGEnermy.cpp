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
			Destroy(); // 비긴 플레이때 있다는 것은 이미 죽음처리가 한번이라도 된적이 있는 것이니 바로 삭제.
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
	//여기에서 따로 호출할 함수 실행.
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