// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGEnermy.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGUnit.h"
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
		bDead = GM->GetSaveEnermyUnits(FieldEnermyNumber);
		if (bDead)
		{
			Destroy(); // 비긴 플레이때 있다는 것은 이미 죽음처리가 한번이라도 된적이 있는 것이니 바로 삭제.
		}
		else
		{
			DropStruct = GM->GetFieldEnermyDropStruct(FieldEnermyNumber);
		}

	}	
}

// Called every frame
void AJRPGEnermy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJRPGEnermy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AJRPGEnermy::EnermyCollisionOverlap(AJRPGPlayerController* PC)
{
	if (PC)
	{
		OwnerController = PC;
		PC->PlayBattleMode(EnermyUnits);
		GM->CurrentBattleEnermy = this;
	}
}


void AJRPGEnermy::PlayerCollisionOverlap(AJRPGUnit* PlayerUnit)
{
	if (PlayerUnit)
	{
		OwnerController = PlayerUnit->OwnerController;
		PlayerUnit->OwnerController->PlayBattleMode(EnermyUnits);
		GM->CurrentBattleEnermy = this;
	}
}

void AJRPGEnermy::FieldEnermyDead()
{
	DeadUnit();
	bDead = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GM->SetSaveEnermyUnits(this);
	PlayAnimMontage(DeadMontage);	
}



void AJRPGEnermy::DropItem_Implementation()
{
	if (OwnerController)
	{
		for (int32 CharNum : OwnerController->CurrentParty)
		{
			OwnerController->AddCharExp(CharNum, DropStruct.DropExp);
			_DEBUG(" %d Add Exp : %d", CharNum, DropStruct.DropExp);
		}
	}
}

void AJRPGEnermy::DeadUnit_Implementation()
{
	DropItem();
}