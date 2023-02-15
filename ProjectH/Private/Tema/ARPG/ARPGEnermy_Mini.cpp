// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGEnermy_Mini.h"
#include "Tema/ARPG/AttackClass.h"
#include "Tema/ARPG/ARPGAttackComponent.h"
#include "Tema/ARPG/AI/ARPG_EnermyAnimInstance.h"


AARPGEnermy_Mini::AARPGEnermy_Mini()
{
	PrimaryActorTick.bCanEverTick = true;
	EnermyType = EEnermyType::Unit;
}

void AARPGEnermy_Mini::BeginPlay()
{
	Super::BeginPlay();

	if (BP_Weapon)
	{

		Weapon = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_Weapon, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		if (Weapon)
		{
			Weapon->OwnerUnit = this;
			Weapon->SetOwner(this);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);

			Weapon->FinishSpawning(FTransform());
		}

	}

	if (BP_Shield)
	{
		Shield = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_Shield, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		if (Shield)
		{
			Shield->OwnerUnit = this;
			Shield->SetOwner(this);
			BlockingDEF = Shield->BlockingDEF;
			Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ShieldSockName);

			Shield->FinishSpawning(FTransform());
		}
	}
}

void AARPGEnermy_Mini::SetHiddenActor(bool bFlag)
{
	SetActorHiddenInGame(bFlag);
	if (Weapon)
	{
		Weapon->SetActorHiddenInGame(bFlag);
	}

	if (Shield)
	{
		Shield->SetActorHiddenInGame(bFlag);
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
	}
}


float AARPGEnermy_Mini::TakeDamageCalculator(AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser)
{
	if (bDeath || bSpecialAttackMode)
	{
		return 0.0f;
	}

	float Damaged = DamageCauser->CalculDamage(DamageWeapon->WeaponDamage * DamageWeapon->Charge);
	float CurrentHP = UnitState.HP;
	bool bBlockingHit = false;

	//���� ���ε��� �̿��ؼ� ���� ������ ���ε��� ��´�.
	Super::TakeDamageCalculator(DamageWeapon, DamageEvent, EventInstigator, DamageCauser);

	if (bBlocking)
	{
		if (TargetDotProduct(DamageCauser->GetActorLocation(), 0.7)) // 45�� ����
		{
			float APDMG = DamageCauser->CalculAPDamage(DamageWeapon->WeaponAP_DMG);
			Damaged = Damaged - (Damaged * BlockingDEF); // BlockingDEF�� 0.0~1.0���� �Ǿ��ִ�.
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
		OnDamage.Broadcast(Damaged);
	}
	else 
	{	
		Hit(bBlockingHit);
		if (Damaged > 0.f)
		{
			CurrentHP -= Damaged;
			UnitState.SetTakeDamageHP(CurrentHP);
			OnDamage.Broadcast(Damaged);
		}
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

	if (!bBlockingHit)
	{
		bool bHitMontagePlay = Super::Hit(bBlockingHit); // ���ε� �˻�
		// ���� ��� AP�� �׷α� �����̰�, ���ε��� ���۾Ƹӳ� �ƴϳ��� �����̴�.

		if (bHitMontagePlay)
		{
			bDontMoving = true;
			bHitting = true;
			bParringHit = false;

			if (bAttacking)
			{
				WeaponOverlapEnd();
				//AttackEnd();
			}

		EnermyAnimInstance->PlayHitMontage(EEnermy_Mini_Mode::BattleMode);	
		}
		else
		{
			//HitEnd();
		}
	}
	else
	{
		bDontMoving = true;
		bHitting = true;
		bParringHit = false;

		EnermyAnimInstance->PlayHitMontage(EEnermy_Mini_Mode::BlockingMode);
	}

	bMoving = false;

	return true;
}


// ��Ʋ��尡 ���� �Ǿ����� ����Ǵ� �Լ�. true�� ��Ʋ ���۽� ��������� ���� ���� ����.
// �ش� Ŭ������ �׷��� ���� �⺻ ���̹Ƿ� �׳� ��Ʋ��峡���� �������ΰ� Ǯ�� ����.
void AARPGEnermy_Mini::SetBattleMode(bool bFlag)
{
	if (bBattleMode == bFlag) // �̰� true�� �̹� �ѹ��̻� ���� �Ǿ��ٴ� ��
		return;

	// bBattleMode ���� �ٲ��ָ鼭 ���ÿ� ���ε� �ʱ�ȭ.
	Super::SetBattleMode(bFlag);

	if (!bFlag)
	{
		Guard(false);
	}
}

void AARPGEnermy_Mini::HitEnd()
{
	bDontMoving = false;
	bHitting = false;
	bParringHit = false;

	AttackEnd();
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

	bAttacking = true;
	Attacks[index]->PlayAttack(GetWorld());
	EnermyAnimInstance->CurrentEffects = Attacks[index]->Effects;
	EnermyAnimInstance->CurrentSounds = Attacks[index]->Sounds;
	EnermyAnimInstance->CurrentAttackSounds = Attacks[index]->AttackSounds;
	EnermyAnimInstance->PlayAttackMontage(Attacks[index]->AttackMontage);
}

void AARPGEnermy_Mini::SetWeaponCollision(bool bFlag)
{
	Weapon->SetWeaponCollision(bFlag);
	if (bFlag)
		Weapon->PlayWeaponSound(EWeaponSFX::SwingSFX);
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
	DeathReset();
	Super::Death(); // ���⼭ ��Ʈ�ѷ� �� ���� ����	
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
	OnAttack.Clear();
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
	bDontLockOn = false;

	if (!bHitting) // bHitting�� true�� HitEnd���� ���� �ִϸ��̼��� ������ false.
		bDontMoving = false;

	OnAttack.Broadcast();
	OnAttack.Clear();
}

void AARPGEnermy_Mini::WeaponOverlapEnd()
{
	SetWeaponCollision(false);
	Weapon->AttackEnd();
}



