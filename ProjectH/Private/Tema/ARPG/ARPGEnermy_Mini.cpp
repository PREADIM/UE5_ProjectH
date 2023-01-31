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

//float AARPGEnermy_Mini::TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	if (bDeath || bSpecialAttackMode)
//	{
//		return 0.0f;
//	}
//
//	float Damaged = DamageAmount;
//	float CurrentHP = UnitState.HP;
//	bool bBlockingHit = false;
//
//	if (bBlocking)
//	{
//		if (TargetDotProduct(DamageCauser->GetActorLocation(), 0.7)) // 45�� ����
//		{
//			Damaged = DamageAmount - (DamageAmount * BlockingDEF); // BlockingDEF�� 0.0~1.0���� �Ǿ��ִ�.
//			TakeDamageAP(APDamage);
//			bBlockingHit = true;
//		}
//	}
//
//	if (CurrentHP <= Damaged)
//	{
//		Damaged = CurrentHP; // ���� ü���� �� ��Ʈ ������
//		CurrentHP = 0.f;
//		UnitState.SetTakeDamageHP(CurrentHP);
//		Death();
//	}
//	else
//	{
//		Hit(bBlockingHit);
//		if (Damaged > 0.f)
//		{
//			CurrentHP -= Damaged;
//			UnitState.SetTakeDamageHP(CurrentHP);
//		}
//	}
//
//	Super::TakeDamageCalculator(APDamage, Damaged, DamageEvent, EventInstigator, DamageCauser);
//
//	return Damaged;
//}


float AARPGEnermy_Mini::TakeDamageCalculator(AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDeath || bSpecialAttackMode)
	{
		return 0.0f;
	}

	float TotalDamage = CalculDamage(DamageWeapon->WeaponDamage * DamageWeapon->Charge);

	float Damaged = TotalDamage;
	float CurrentHP = UnitState.HP;
	bool bBlockingHit = false;

	//���� ���ε��� �̿��ؼ� ���� ������ ���ε��� ��´�.
	Super::TakeDamageCalculator(DamageWeapon, DamageEvent, EventInstigator, DamageCauser);

	if (bBlocking)
	{
		if (TargetDotProduct(DamageCauser->GetActorLocation(), 0.7)) // 45�� ����
		{
			float APDMG = CalculAPDamage(DamageWeapon->WeaponAP_DMG);
			Damaged = TotalDamage - (TotalDamage * BlockingDEF); // BlockingDEF�� 0.0~1.0���� �Ǿ��ִ�.
			TakeDamageAP(APDMG);
			bBlockingHit = true;
		}
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
		Hit(bBlockingHit);
		if (Damaged > 0.f)
		{
			CurrentHP -= Damaged;
			UnitState.SetTakeDamageHP(CurrentHP);
		}
	}

	if (Damaged > 0.f)
	{
		OnDamage.Broadcast(Damaged);
	}

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
		UnitState.SetAP(CurrentAP);
	}
}

bool AARPGEnermy_Mini::Hit(bool bBlockingHit)
{
	if (!EnermyAnimInstance)
		return false;

	WeaponOverlapEnd();
	AttackEnd();

	bHitting = true;
	bParringHit = false;

	if (!bBlockingHit)
	{
		bool bHitMontagePlay = Super::Hit(bBlockingHit); // ���ε� �˻�
		// ���� ��� AP�� �׷α� �����̰�, ���ε��� ���۾Ƹӳ� �ƴϳ��� �����̴�.

		if (bHitMontagePlay)
		{
			EnermyAnimInstance->PlayHitMontage(EEnermy_Mini_Mode::BattleMode);
		}
	}
	else
	{
		EnermyAnimInstance->PlayHitMontage(EEnermy_Mini_Mode::BlockingMode);
	}

	bMoving = false;

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

void AARPGEnermy_Mini::ParringHit(AARPGUnitBase* InstigatorActor)
{
	bMoving = false;
	bParringHit = true;
	bHitting = true;
	bBlocking = false;
	bBlockMode = false;
	OnAttack.Broadcast();
	WeaponOverlapEnd();
	AttackEnd();
	EnermyAnimInstance->ParringInstigatorUnit = InstigatorActor;
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


