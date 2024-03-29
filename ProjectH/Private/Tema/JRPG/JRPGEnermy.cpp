// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGEnermy.h"
#include "Tema/JRPG/JRPGFieldAIController.h"
#include "Tema/JRPG/JRPGGameMode.h"

// Sets default values
AJRPGFieldEnermy::AJRPGFieldEnermy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.f, 0.f);
	bDead = false;
}

// Called when the game starts or when spawned
void AJRPGFieldEnermy::BeginPlay()
{
	Super::BeginPlay();

	DefaultTransform = GetActorTransform();
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


void AJRPGFieldEnermy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJRPGFieldEnermy::FieldEnermyDead()
{
	DeadUnit();
	bDead = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GM->SetSaveEnermyUnits(this);
	PlayAnimMontage(DeadMontage);
	OwnerAIController->OnUnPossess();
}


void AJRPGFieldEnermy::DeadUnit()
{
	if(bBPBindFunc)
		BPBindFunc_DeadUnit();
	//여기에서 따로 호출할 함수 실행.
}


void AJRPGFieldEnermy::ReturnToField()
{
	if (OwnerAIController)
	{
		SetFieldEnermyCC();
		SetActorTransform(DefaultTransform);
	}
}


void AJRPGFieldEnermy::BattleStart()
{
	if (OwnerAIController)
		OwnerAIController->BattleStart();
}


class UBehaviorTree* AJRPGFieldEnermy::GetBT()
{
	if (BT)
		return BT;
	else
		return nullptr;
}


void AJRPGFieldEnermy::SetFieldEnermyCC()
{
	bAnimCC = true;
	bCC = true;
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AJRPGFieldEnermy::StunEnd, 4.f, false);
}

void AJRPGFieldEnermy::StunEnd()
{
	bCC = false;
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	OwnerAIController->ReturnToField();
	/* 애님 블프에서 노티파이로 bAnimCC를 false해주며 애니메이션 종료 */
}