// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGEnermy_FstBoss.h"
#include "Tema/ARPG/AttackClass.h"
#include "Tema/ARPG/ARPGAttackComponent.h"
#include "Tema/ARPG/AI/FstBoss/ARPG_FstBossAnimInstance.h"
#include "Tema/ARPG/ARPGUnit.h"

AARPGEnermy_FstBoss::AARPGEnermy_FstBoss()
{
	PrimaryActorTick.bCanEverTick = true;
	UnitState.NormallyAP = 500.f;
	EnermyType = EEnermyType::Boss;
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

			LeftWeapon->SetOwner(this);
			RightWeapon->SetOwner(this);
			TwinWeapon->SetOwner(this);

			LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_l_Collision"));
			RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_r_Collision"));
			TwinWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("TwinWeaponSocket"));

			LeftWeapon->FinishSpawning(FTransform());
			RightWeapon->FinishSpawning(FTransform());
			TwinWeapon->FinishSpawning(FTransform());
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

float AARPGEnermy_FstBoss::TakeDamageCalculator(AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser)
{
	if (bDeath || bSpecialAttackMode)
	{
		return 0.0f;
	}

	
	float APDMG = DamageCauser->CalculAPDamage(DamageWeapon->WeaponAP_DMG);
	float Damaged = DamageCauser->CalculDamage(DamageWeapon->WeaponDamage * DamageWeapon->Charge);
	float CurrentHP = UnitState.HP;

	//���� ���ε��� �̿��ؼ� ���� ������ ���ε��� ��´�.
	Super::TakeDamageCalculator(DamageWeapon, DamageEvent, EventInstigator, DamageCauser);


	if (CurrentHP <= Damaged)
	{
		Damaged = CurrentHP; // ���� ü���� �� ��Ʈ ������
		CurrentHP = 0.f;
		UnitState.SetTakeDamageHP(CurrentHP);
		Death();
		OnDamage.Broadcast(Damaged);
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
			OnDamage.Broadcast(Damaged);
		}
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

	bDontMoving = true;
	bHitting = true;
	bParringHit = false;

	if (bAttacking)
	{
		WeaponOverlapEnd(3);
		AttackEnd();
	}

	bool bHitMontagePlay = Super::Hit(bBlockingHit); // ���ε� �˻�
	// ���� ��� AP�� �׷α� �����̰�, ���ε��� ���۾Ƹӳ� �ƴϳ��� �����̴�.

	if (bHitMontagePlay)
	{
		FstBossAnimInstance->PlayHitMontage();
		bMoving = false;
	}
	else
	{
		HitEnd();
	}

	return true;
}

// ��Ʋ��尡 ���� �Ǿ����� ����Ǵ� �Լ�. true�� ��Ʋ ���۽� ��������� ���� ���� ����.
void AARPGEnermy_FstBoss::SetBattleMode(bool bFlag)
{
	Super::SetBattleMode(bFlag);

	// �������� �Ʒ��� ���� ü���� ��Ÿ���� UI ����
	if (PlayerUnit)
	{
		if(!PlayerUnit->IsBossHPWidget())
			PlayerUnit->SetBossHPWidget(bFlag, this);
	}

}

void AARPGEnermy_FstBoss::HitEnd()
{
	bDontMoving = false;
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
	DeathReset();
	Super::Death(); // ���⼭ ��Ʈ�ѷ� �� ���� ����
	FstBossAnimInstance->PlayDeadMontage();
}

void AARPGEnermy_FstBoss::ParringHit(AARPGUnitBase* InstigatorActor)
{
	bMoving = false;
	bParringHit = true;
	bHitting = true;
	WeaponOverlapEnd(3);
	AttackEnd();
	FstBossAnimInstance->ParringInstigatorUnit = InstigatorActor;
	FstBossAnimInstance->PlayParringHitMontage();
}

void AARPGEnermy_FstBoss::DeathCollsionEnabled()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	WeaponOverlapEnd(3);
	AttackEnd();
	if (PlayerUnit)
	{
		PlayerUnit->SetBossHPWidget(false, this);
	}
}

void AARPGEnermy_FstBoss::AttackEnd()
{
	bAttacking = false;
	bDontLockOn = false;

	if(!bHitting) // bHitting�� true�� HitEnd���� ���� �ִϸ��̼��� ������ false.
		bDontMoving = false;

	OnAttack.Broadcast();
	OnAttack.Clear();
}

void AARPGEnermy_FstBoss::WeaponOverlapEnd(int32 Num)
{
	switch (Num)
	{
	case 0:
		SetWeaponCollision(false, 0);
		TwinWeapon->AttackEnd();
		break;
	case 1:
		SetWeaponCollision(false, 1);
		LeftWeapon->AttackEnd();
		break;
	case 2:
		SetWeaponCollision(false, 2);
		RightWeapon->AttackEnd();
		break;
	case 3:
		SetWeaponEndCollision();
		break;
	}
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


	//0��� 1�޼� 2������
	switch (index)
	{
	case 0:
		TwinWeapon->SetWeaponCollision(bFlag);
		if (bFlag)
			TwinWeapon->PlayWeaponSound(EWeaponSFX::SwingSFX);
		break;
	case 1:
		LeftWeapon->SetWeaponCollision(bFlag);
		if (bFlag)
			LeftWeapon->PlayWeaponSound(EWeaponSFX::SwingSFX);
		break;
	case 2:
		RightWeapon->SetWeaponCollision(bFlag);
		if (bFlag)
			RightWeapon->PlayWeaponSound(EWeaponSFX::SwingSFX);
		break;
	}
}

