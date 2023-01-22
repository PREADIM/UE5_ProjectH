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
			BlockingDEF = Shield->BlockingDEF;
			Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ShieldSockName);
		}
	}
}

void AARPGEnermy_Mini::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void AARPGEnermy_Mini::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetMesh()) // ���� C++ Ŭ������ ��ӹ��� ������ ������â�� ������鼭 null���� �ֱ⶧��.(����)
	{
		EnermyAnimInstance = Cast<UARPG_EnermyAnimInstance>(GetMesh()->GetAnimInstance());
		if (EnermyAnimInstance == nullptr)
		{
			_DEBUG("Not AI Has AnimInstance");
		}
	}
}

float AARPGEnermy_Mini::TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDeath || bSpecialAttackMode)
	{
		return 0.0f;
	}

	float Damaged = DamageAmount;
	float CurrentHP = UnitState.HP;

	if (bBlocking)
	{
		Damaged = DamageAmount - (DamageAmount * BlockingDEF); // BlockingDEF�� 0.0~1.0���� �Ǿ��ִ�.
		TakeDamageAP(APDamage);
	}

	if (CurrentHP <= Damaged)
	{
		Damaged = CurrentHP; // ���� ü���� �� ��Ʈ ������
		CurrentHP = 0.f;
		UnitState.SetTakeDamageHP(CurrentHP);
		Death();
	}
	else
	{
		Hit();
		if (Damaged > 0.f)
		{
			CurrentHP -= Damaged;
			UnitState.SetTakeDamageHP(CurrentHP);
		}
	}

	Super::TakeDamageCalculator(APDamage, Damaged, DamageEvent, EventInstigator, DamageCauser);

	return Damaged;
}


void AARPGEnermy_Mini::TakeDamageAP(float Damage)
{
	float CurrentAP = UnitState.AP;
	if (CurrentAP <= Damage)
	{
		CurrentAP = 0.f;
		UnitState.SetAP(CurrentAP);
		ZeroAP();
	}
	else
	{
		CurrentAP -= Damage;
		Hit();
		UnitState.SetAP(CurrentAP);
	}
}

void AARPGEnermy_Mini::Hit()
{
	bHitting = true;
	bParringHit = false;
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

	if (bParring)
		return false;

	return true;
}

// ��Ʋ��尡 ���� �Ǿ����� ����Ǵ� �Լ�. true�� ��Ʋ ���۽� ��������� ���� ���� ����.
// �ش� Ŭ������ �׷��� ���� �⺻ ���̹Ƿ� �׳� ��Ʋ��峡���� �������ΰ� Ǯ�� ����.
void AARPGEnermy_Mini::ChangeBattleMode(bool bFlag)
{
	if (!bFlag)
	{
		Guard(false);
	}
}

void AARPGEnermy_Mini::HitEnd()
{
	bHitting = false;
	bParringHit = false;
}

// ���带 �Ұ����� ���� ������ ����. �⺻������ Guard���� �ش� �Լ��� �����ϴ� ���.
void AARPGEnermy_Mini::SetBlocking(bool bFlag)
{
	if (!EnermyAnimInstance)
		return;

	if (bFlag)
	{
		bBlockMode = true;
		EnermyAnimInstance->PlayBlockingMontage();
	}
	else
	{
		bBlockMode = false;
		bBlocking = false;
	}

}

//���ʹ��� ZeroAP�� ���� �׷α� ���¸� ���Ѵ�.
void AARPGEnermy_Mini::ZeroAP()
{
	Super::ZeroAP();
	Guard(false);
	EnermyAnimInstance->ZeroAP();
}

void AARPGEnermy_Mini::DeathWeaponSimulate()
{
	// ������ ����ִ� ���⸦ ���Ͽ��� ��� ������ �ù�

	FDetachmentTransformRules Rule(EDetachmentRule::KeepWorld, false);

	if (Weapon)
	{
		Weapon->DetachFromActor(Rule);
		Weapon->SetPhysics();
	}

	if (Shield)
	{
		Shield->DetachFromActor(Rule);
		Shield->SetPhysics();
	}
}



// �̰� ����Ǿ��ٴ� ���� �ᱹ BT���� AttackDistance�� �˻��ؼ� �������ٴ� ���̴�.
void AARPGEnermy_Mini::Attack(int32 index)
{
	if (bParringHit) // �и����ѰŸ� ���ʿ� ����
		return;

	PlayAttack(index);
}


void AARPGEnermy_Mini::PlayAttack(int32 index)
{
	const TArray<UAttackClass*>& Attacks = AttackComponent->GetAttacks();

	if (!Attacks.IsValidIndex(index))
	{
		return;
	}


	_DEBUG("Attack");

	bAttacking = true;
	Attacks[index]->PlayAttack(GetWorld());
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
	if (bParringHit) // �и����ѰŸ� ���ʿ� ����
		return;

	SetBlocking(bFlag);
}

void AARPGEnermy_Mini::Parring(bool bFlag)
{
	bParring = bFlag;
}

void AARPGEnermy_Mini::DeathReset()
{
	bMoving = false;
	WeaponOverlapEnd();
	AttackEnd();
	Guard(false);
}

void AARPGEnermy_Mini::Death()
{
	Super::Death(); // ���⼭ ��Ʈ�ѷ� �� ���� ����
	DeathReset();
	EnermyAnimInstance->PlayDeadMontage();	
}

void AARPGEnermy_Mini::ParringHit()
{
	bMoving = false;
	bParringHit = true;
	bHitting = true;
	bBlocking = false;
	bBlockMode = false;
	OnAttack.Broadcast();
	EnermyAnimInstance->PlayParringHitMontage();
}

void AARPGEnermy_Mini::DeathCollsionEnabled()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	SetWeaponCollision(false);
	Weapon->AttackEnd();
}



