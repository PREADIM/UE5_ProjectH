// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGEnermy.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/BattleUI/DropItemWidget.h"
#include "Tema/JRPG/BattleUI/DropExpWidget.h"

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
			Destroy(); // ��� �÷��̶� �ִٴ� ���� �̹� ����ó���� �ѹ��̶� ������ �ִ� ���̴� �ٷ� ����.
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



void AJRPGEnermy::DropItem()
{
	if (OwnerController)
	{
		for (int32 CharNum : OwnerController->CurrentParty)
		{
			OwnerController->AddCharExp(CharNum, DropStruct.DropExp);
			//_DEBUG(" %d Add Exp : %f", CharNum, DropStruct.DropExp);
		}

		if (OwnerController->DropExpWidget)
		{
			OwnerController->DropExpWidget->Init(DropStruct.DropExp);
			OwnerController->DropExpWidget->AddToViewport();
		}

		_DEBUG("DropStruct Char Num : %d", DropStruct.DropCharNum);

		if (DropStruct.DropCharNum != 0)
		{
			OwnerController->AddDropChar(DropStruct.DropCharNum);
			FString Name = OwnerController->GetUnitUI(DropStruct.DropCharNum)->CharName;
			if (OwnerController->DropCharWidget)
			{
				OwnerController->DropCharWidget->Init(Name);
				OwnerController->DropCharWidget->AddToViewport();
			}

			if (!GM->bPartyTutorial) // ĳ���� ��������� ��Ƽ ���� �غ���� Ʃ�丮�� ����.
			{
				GM->PartyTutorialStart();
			}
		}
	}
}

void AJRPGEnermy::DeadUnit()
{
	DropItem();

	if(bBPBindFunc)
		BPBindFunc();
	//���⿡�� ���� ȣ���� �Լ� ����.
}