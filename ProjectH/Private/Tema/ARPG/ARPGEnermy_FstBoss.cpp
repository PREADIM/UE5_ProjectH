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

	if (BP_LeftWeapon && BP_RightWeapon)
	{
		LeftWeapon = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_LeftWeapon, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		RightWeapon = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_LeftWeapon, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);

		if (LeftWeapon && RightWeapon)
		{
			LeftWeapon->OwnerUnit = this;
			RightWeapon->OwnerUnit = this;

			LeftWeapon->FinishSpawning(FTransform());
			RightWeapon->FinishSpawning(FTransform());

			LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_l_Collision"));
			RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_r_Collision"));
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

	if (GetMesh()) // 가끔 C++ 클래스를 상속받은 블프가 디테일창이 사라지면서 null값을 넣기때문.(버그)
	{
		FstBossAnimInstance = Cast<UARPG_FstBossAnimInstance>(GetMesh()->GetAnimInstance());
		if (FstBossAnimInstance == nullptr)
		{
			_DEBUG("Not AI Has AnimInstance");
		}
	}
}

float AARPGEnermy_FstBoss::TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDeath || bSpecialAttackMode)
	{
		return 0.0f;
	}

	float Damaged = DamageAmount;
	float CurrentHP = UnitState.HP;

	if (CurrentHP <= Damaged)
	{
		Damaged = CurrentHP; // 남은 체력이 곧 라스트 데미지
		CurrentHP = 0.f;
		UnitState.SetTakeDamageHP(CurrentHP);
		Death();
	}
	else
	{
		Hit(false); // 이 보스는 가드가 없다.
		if (Damaged > 0.f)
		{
			CurrentHP -= Damaged;
			UnitState.SetTakeDamageHP(CurrentHP);
			TakeDamageAP(APDamage);
			// 보스의 ZeroAP는 그로기를 뜻한다.
		}
	}

	Super::TakeDamageCalculator(APDamage, Damaged, DamageEvent, EventInstigator, DamageCauser);

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

void AARPGEnermy_FstBoss::Hit(bool bBlockingHit)
{
	WeaponOverlapEnd();
	AttackEnd();

	bHitting = true;
	bParringHit = false;

	FstBossAnimInstance->PlayHitMontage();	
	bMoving = false;
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
	FstBossAnimInstance->PlayAttackMontage(Attacks[index]->AttackMontage);
}

void AARPGEnermy_FstBoss::Death()
{
	Super::Death(); // 여기서 컨트롤러 및 모든걸 해제
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
	// 죽으면 들고있는 무기를 소켓에서 떼어내 피직스 시뮬
	// 이보스는 들고 있는 무기가 내장형이기 때문에 딱히 없음.
}


//보스의 경우 ZeroAP는 다르게 사용
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
	SetWeaponCollision(false);
	LeftWeapon->AttackEnd();
	RightWeapon->AttackEnd();
}

void AARPGEnermy_FstBoss::SetWeaponCollision(bool bFlag)
{
	if (!LeftWeapon || !RightWeapon)
	{
		return;
	}


	LeftWeapon->SetWeaponCollision(bFlag);
	RightWeapon->SetWeaponCollision(bFlag);
}

