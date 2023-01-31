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

	if (GetMesh()) // 가끔 C++ 클래스를 상속받은 블프가 디테일창이 사라지면서 null값을 넣기때문.(버그)
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
//		Damaged = CurrentHP; // 남은 체력이 곧 라스트 데미지
//		CurrentHP = 0.f;
//		UnitState.SetTakeDamageHP(CurrentHP);
//		Death();
//	}
//	else
//	{
//		Hit(false); // 이 보스는 가드가 없다.
//		if (Damaged > 0.f)
//		{
//			CurrentHP -= Damaged;
//			UnitState.SetTakeDamageHP(CurrentHP);
//			TakeDamageAP(APDamage);
//			// 보스의 ZeroAP는 그로기를 뜻한다.
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

	//무기 강인도를 이용해서 맞은 액터의 강인도를 깎는다.
	Super::TakeDamageCalculator(DamageWeapon, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= Damaged)
	{
		Damaged = CurrentHP; // 남은 체력이 곧 라스트 데미지
		CurrentHP = 0.f;
		UnitState.SetTakeDamageHP(CurrentHP);
		Death();
	}
	else
	{
		Hit(false); // 가드가 없다.
		if (Damaged > 0.f)
		{
			CurrentHP -= Damaged;
			UnitState.SetTakeDamageHP(CurrentHP);
			TakeDamageAP(APDMG);
			// 보스의 ZeroAP는 그로기를 뜻한다.
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

	bool bHitMontagePlay = Super::Hit(bBlockingHit); // 강인도 검사
	// 적의 경우 AP는 그로기 여부이고, 강인도는 슈퍼아머냐 아니냐의 차이이다.

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


	//bHand false 왼손 true 오른손
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

