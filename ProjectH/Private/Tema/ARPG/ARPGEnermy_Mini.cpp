// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGEnermy_Mini.h"
#include "Tema/ARPG/AttackClass.h"
#include "Tema/ARPG/ARPGAttackComponent.h"
#include "Tema/ARPG/Weapon/ARPGWeapon_Sword.h"
#include "Tema/ARPG/Shield/ARPGShield_Normal.h"

AARPGEnermy_Mini::AARPGEnermy_Mini()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARPGEnermy_Mini::BeginPlay()
{
	Super::BeginPlay();

	if (BP_Weapon)
	{
		Weapon = GetWorld()->SpawnActor<AARPGWeapon>(BP_Weapon);
		if (Weapon)
		{
			Weapon->OwnerUnit = this;
			Weapon->SetOwner(this);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
		}

	}

	if (BP_Shield)
	{
		Shield = GetWorld()->SpawnActor<AARPGShield>(BP_Shield);
		if (Shield)
		{
			Shield->OwnerUnit = this;
			Shield->SetOwner(this);
			Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, ShieldSockName);
		}
	}



}

void AARPGEnermy_Mini::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARPGEnermy_Mini::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AARPGEnermy_Mini::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


// 이게 실행되었다는 것은 결국 BT에서 AttackDistance로 검사해서 문제없다는 뜻이다.
void AARPGEnermy_Mini::Attack(int32 index)
{
	PlayAttack(index);
}


void AARPGEnermy_Mini::PlayAttack(int32 index)
{
	const TArray<UAttackClass*>& Attacks = AttackComponent->GetAttacks();

	if (!Attacks.IsValidIndex(index))
	{
		return;
	}

	bAttacking = true;
	bMoving = false;
	PlayAnimMontage(Attacks[index]->AttackMontage);
	Attacks[index]->PlayAttack();

}

void AARPGEnermy_Mini::SetWeaponCollision(bool bFlag)
{
	if (bFlag)
	{
		Weapon->WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		_DEBUG("Collision On");
	}
	else
	{
		Weapon->WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		_DEBUG("Collision Off");
	}
}

void AARPGEnermy_Mini::SetShieldCollision(bool bFlag)
{

}


void AARPGEnermy_Mini::Garud()
{

}

void AARPGEnermy_Mini::Parring()
{

}


// 몽타주 끝나면 하면 될듯.
void AARPGEnermy_Mini::AttackEnd()
{
	bAttacking = false;
	OnAttack.Broadcast();
	WeaponOverlapEnd();
}

void AARPGEnermy_Mini::WeaponOverlapEnd()
{
	Weapon->AttackEnd();
}



