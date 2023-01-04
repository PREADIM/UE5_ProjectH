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
	// 따로 이런 함수를 만든 이유는, 쉴드 뿐만아니라 특정 상황에서 못때리는 상태 일 수도 있으므로,
	if (bBlocking)
		return false;

	if (bDeath)
		return false;

	return true;
}

// 배틀모드가 실행 되었을때 실행되는 함수. true면 배틀 시작시 버프라던가 무언가 실행 가능.
// 해당 클래스는 그런거 없는 기본 적이므로 그냥 배틀모드끝나면 가드중인거 풀기 정도.
void AARPGEnermy_Mini::ChangeBattleMode(bool bFlag)
{
	//_DEBUG("Enermt Mini ChangeBattleMode");
	if (!bFlag)
	{
		Guard(false);
	}
}

// 가드를 할것인지 안할 것인지 실행. 기본적으로 Guard에서 해당 함수를 실행하는 방식.
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
	Attacks[index]->PlayAttack();
	EnermyAnimInstance->PlayAttackMontage(Attacks[index]->AttackMontage);
}

void AARPGEnermy_Mini::SetWeaponCollision(bool bFlag)
{
	Weapon->SetWeaponCollision(bFlag);
}

void AARPGEnermy_Mini::SetShieldCollision(bool bFlag)
{
	// 쉴드로 공격할때 쓰는정도.
	// 이 클래스는 쓰지 않을가능성 높음.
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


// 몽타주 끝나면 하면 될듯.
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



