// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGEnermy_Mini.h"
#include "Tema/ARPG/AttackClass.h"
#include "Tema/ARPG/ARPGAttackComponent.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "Tema/ARPG/AI/ARPG_EnermyAnimInstance.h"


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
		Shield = GetWorld()->SpawnActor<AARPGWeapon>(BP_Shield);
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

float AARPGEnermy_Mini::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	_DEBUG("Enermy TakeDamage");

	float CurrentHP = UnitState.HP;
	if (CurrentHP <= DamageAmount)
	{
		CurrentHP = 0.f;
		UnitState.SetTakeDamageHP(CurrentHP);
		Death();
		return DamageAmount;
	}

	CurrentHP -= DamageAmount;

	Hit();
	UnitState.SetTakeDamageHP(CurrentHP);

	return DamageAmount;
}


void AARPGEnermy_Mini::TakeDamageAP(float Damage)
{
	float CurrentAP = UnitState.AP;
	if (CurrentAP <= Damage)
	{
		CurrentAP = 0.f;
		UnitState.SetTakeDamageAP(CurrentAP);
		ZeroAP();
		return;
	}
	
	CurrentAP -= Damage;
	Hit();
	UnitState.SetTakeDamageAP(CurrentAP);
}

void AARPGEnermy_Mini::Hit()
{
	if (bBlocking)
	{
		EnermyAnimInstance->PlayHitMontage(EEnermy_Mini_Mode::BlockingMode);
	}
	else
	{
		EnermyAnimInstance->PlayHitMontage(EEnermy_Mini_Mode::BattleMode);
	}
	
	bMoving = false;
	AttackEnd();
}

bool AARPGEnermy_Mini::CanThisDamage()
{	
	// ���� �̷� �Լ��� ���� ������, ���� �Ӹ��ƴ϶� Ư�� ��Ȳ���� �������� ���� �� ���� �����Ƿ�,
	if (bBlocking)
		return false;

	if (bDeath)
		return false;

	return true;
}

// ��Ʋ��尡 ���� �Ǿ����� ����Ǵ� �Լ�. true�� ��Ʋ ���۽� ��������� ���� ���� ����.
// �ش� Ŭ������ �׷��� ���� �⺻ ���̹Ƿ� �׳� ��Ʋ��峡���� �������ΰ� Ǯ�� ����.
void AARPGEnermy_Mini::ChangeBattleMode(bool bFlag)
{
	//_DEBUG("Enermt Mini ChangeBattleMode");
	if (!bFlag)
	{
		Guard(false);
	}
}

// ���带 �Ұ����� ���� ������ ����. �⺻������ Guard���� �ش� �Լ��� �����ϴ� ���.
void AARPGEnermy_Mini::SetBlocking(bool bFlag)
{
	if (!EnermyAnimInstance)
		return;

	if (bFlag)
	{
		bBlockMode = true;
	}
	else
	{
		bBlockMode = false;
		bBlocking = false;
	}

}

void AARPGEnermy_Mini::ZeroAP()
{
	Guard(false);
	EnermyAnimInstance->ZeroAP();
}



// �̰� ����Ǿ��ٴ� ���� �ᱹ BT���� AttackDistance�� �˻��ؼ� �������ٴ� ���̴�.
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
	Attacks[index]->PlayAttack();
	EnermyAnimInstance->PlayAttackMontage(Attacks[index]->AttackMontage);
}

void AARPGEnermy_Mini::SetWeaponCollision(bool bFlag)
{
	Weapon->SetWeaponCollision(bFlag);
}

void AARPGEnermy_Mini::SetShieldCollision(bool bFlag)
{
	// ����� �����Ҷ� ��������.
	// �� Ŭ������ ���� �������ɼ� ����.
}


void AARPGEnermy_Mini::Guard(bool bFlag)
{
	if (bFlag)
	{
		SetEnermyMoveMode(EEnermyMoveMode::None);
	}
	SetBlocking(bFlag);
}

void AARPGEnermy_Mini::Parring(bool bFlag)
{
	bParring = bFlag;
}

void AARPGEnermy_Mini::Death()
{
	EnermyAnimInstance->PlayDeadMontage();
}


// ��Ÿ�� ������ �ϸ� �ɵ�.
void AARPGEnermy_Mini::AttackEnd()
{
	bAttacking = false;
	OnAttack.Broadcast();
	OnAttack.Clear();
}

void AARPGEnermy_Mini::WeaponOverlapEnd()
{
	_DEBUG("Enermy Weapon AttackEnd");
	SetWeaponCollision(false);
	Weapon->AttackEnd();
}



