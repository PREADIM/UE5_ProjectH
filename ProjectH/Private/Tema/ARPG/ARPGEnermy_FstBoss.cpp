// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGEnermy_FstBoss.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "Tema/ARPG/AttackClass.h"
#include "Tema/ARPG/ARPGAttackComponent.h"
#include "Tema/ARPG/AI/FstBoss/ARPG_FstBossAnimInstance.h"

AARPGEnermy_FstBoss::AARPGEnermy_FstBoss()
{
	PrimaryActorTick.bCanEverTick = true;
	UnitState.NormallyAP = 500.f;
}

void AARPGEnermy_FstBoss::BeginPlay()
{
	Super::BeginPlay();

	if (BP_LeftWeapon && BP_RightWeapon && BP_TwinWeapon)
	{
		LeftWeapon = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_LeftWeapon, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		RightWeapon = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_RightWeapon, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		TwinWeapon = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_TwinWeapon, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);

		if (LeftWeapon && RightWeapon && TwinWeapon)
		{
			LeftWeapon->OwnerUnit = this;
			RightWeapon->OwnerUnit = this;
			TwinWeapon->OwnerUnit = this;

			LeftWeapon->FinishSpawning(FTransform());
			RightWeapon->FinishSpawning(FTransform());
			TwinWeapon->FinishSpawning(FTransform());

			LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_l_Collision"));
			RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_r_Collision"));
			TwinWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("TwinWeaponSocket"));
		}
	}
}

void AARPGEnermy_FstBoss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AARPGEnermy_FstBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetMesh()) // ���� C++ Ŭ������ ��ӹ��� ������ ������â�� ������鼭 null���� �ֱ⶧��.(����)
	{
		FstBossAnimInstance = Cast<UARPG_FstBossAnimInstance>(GetMesh()->GetAnimInstance());
		if (FstBossAnimInstance == nullptr)
		{
			_DEBUG("FstBoss Not AnimInstance");
		}
	}
}

//float AARPGEnermy_FstBoss::TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	if (bDeath || bSpecialAttackMode)
//	{
//		return 0.0f;
//	}
//
//	float Damaged = DamageAmount;
//	float CurrentHP = UnitState.HP;
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
//		Hit(false); // �� ������ ���尡 ����.
//		if (Damaged > 0.f)
//		{
//			CurrentHP -= Damaged;
//			UnitState.SetTakeDamageHP(CurrentHP);
//			TakeDamageAP(APDamage);
//			// ������ ZeroAP�� �׷α⸦ ���Ѵ�.
//		}
//	}
//
//	Super::TakeDamageCalculator(APDamage, Damaged, DamageEvent, EventInstigator, DamageCauser);
//
//	return Damaged;
//}


float AARPGEnermy_FstBoss::TakeDamageCalculator(AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDeath || bSpecialAttackMode)
	{
		return 0.0f;
	}

	float TotalDamage = CalculDamage(DamageWeapon->WeaponDamage * DamageWeapon->Charge);
	float APDMG = CalculAPDamage(DamageWeapon->WeaponAP_DMG);

	float Damaged = TotalDamage;
	float CurrentHP = UnitState.HP;
	bool bBlockingHit = false;

	//���� ���ε��� �̿��ؼ� ���� ������ ���ε��� ��´�.
	Super::TakeDamageCalculator(DamageWeapon, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= Damaged)
	{
		Damaged = CurrentHP; // ���� ü���� �� ��Ʈ ������
		CurrentHP = 0.f;
		UnitState.SetTakeDamageHP(CurrentHP);
		Death();
	}
	else
	{
		Hit(false); // ���尡 ����.
		if (Damaged > 0.f)
		{
			CurrentHP -= Damaged;
			UnitState.SetTakeDamageHP(CurrentHP);
			TakeDamageAP(APDMG);
			// ������ ZeroAP�� �׷α⸦ ���Ѵ�.
		}
	}

	if (Damaged > 0.f)
	{
		OnDamage.Broadcast(Damaged);
	}

	return Damaged;
}


void AARPGEnermy_FstBoss::TakeDamageAP(float Damage)
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

bool AARPGEnermy_FstBoss::Hit(bool bBlockingHit)
{
	if (!FstBossAnimInstance)
		return false;

	WeaponOverlapEnd();
	AttackEnd();

	bHitting = true;
	bParringHit = false;

	bool bHitMontagePlay = Super::Hit(bBlockingHit); // ���ε� �˻�
	// ���� ��� AP�� �׷α� �����̰�, ���ε��� ���۾Ƹӳ� �ƴϳ��� �����̴�.

	if (bHitMontagePlay)
	{
		FstBossAnimInstance->PlayHitMontage();
		bMoving = false;
	}

	return true;
}

void AARPGEnermy_FstBoss::ChangeBattleMode(bool bFlag)
{

}

void AARPGEnermy_FstBoss::HitEnd()
{
	bHitting = false;
	bParringHit = false;
}

void AARPGEnermy_FstBoss::Attack(int32 index)
{
	if (bParringHit)
		return;

	PlayAttack(index);
}


void AARPGEnermy_FstBoss::PlayAttack(int32 index)
{
	const TArray<UAttackClass*>& Attacks = AttackComponent->GetAttacks();

	if (!Attacks.IsValidIndex(index))
		return;

	bAttacking = true;
	Attacks[index]->PlayAttack(GetWorld());
	FstBossAnimInstance->CurrentEffects = Attacks[index]->Effects;
	FstBossAnimInstance->CurrentSounds = Attacks[index]->Sounds;
	FstBossAnimInstance->PlayAttackMontage(Attacks[index]->AttackMontage);
}

void AARPGEnermy_FstBoss::Death()
{
	Super::Death(); // ���⼭ ��Ʈ�ѷ� �� ���� ����
	DeathReset();
	FstBossAnimInstance->PlayDeadMontage();
}

void AARPGEnermy_FstBoss::ParringHit(AARPGUnitBase* InstigatorActor)
{
	bMoving = false;
	bParringHit = true;
	bHitting = true;
	OnAttack.Broadcast();
	WeaponOverlapEnd();
	AttackEnd();
	FstBossAnimInstance->ParringInstigatorUnit = InstigatorActor;
	FstBossAnimInstance->PlayParringHitMontage();
}

void AARPGEnermy_FstBoss::DeathCollsionEnabled()
{

}

void AARPGEnermy_FstBoss::DeathWeaponSimulate()
{
	// ������ ����ִ� ���⸦ ���Ͽ��� ��� ������ �ù�
	// �̺����� ��� �ִ� ���Ⱑ �������̱� ������ ���� ����.
}


//������ ��� ZeroAP�� �ٸ��� ���
void AARPGEnermy_FstBoss::ZeroAP()
{
	Super::ZeroAP();
	FstBossAnimInstance->ZeroAP();
}

void AARPGEnermy_FstBoss::DeathReset()
{
	bMoving = false;
	WeaponOverlapEnd();
	AttackEnd();
}

void AARPGEnermy_FstBoss::AttackEnd()
{
	bAttacking = false;
	bDontParringAttack = false;
	OnAttack.Broadcast();
	OnAttack.Clear();
}

void AARPGEnermy_FstBoss::WeaponOverlapEnd()
{
	SetWeaponEndCollision();
	TwinWeapon->AttackEnd();
	LeftWeapon->AttackEnd();
	RightWeapon->AttackEnd();
}

void AARPGEnermy_FstBoss::SetWeaponEndCollision()
{
	SetWeaponCollision(false, 0);
	SetWeaponCollision(false, 1);
	SetWeaponCollision(false, 2);
}

void AARPGEnermy_FstBoss::SetWeaponCollision(bool bFlag, int32 index)
{
	if (!LeftWeapon || !RightWeapon || !TwinWeapon)
	{
		return;
	}


	//bHand false �޼� true ������
	switch (index)
	{
	case 0:
		TwinWeapon->SetWeaponCollision(bFlag);
		break;
	case 1:
		LeftWeapon->SetWeaponCollision(bFlag);
		break;
	case 2:
		RightWeapon->SetWeaponCollision(bFlag);
		break;
	}
}

