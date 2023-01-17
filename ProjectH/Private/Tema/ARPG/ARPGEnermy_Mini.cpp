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
			Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ShieldSockName);
		}
	}



}

void AARPGEnermy_Mini::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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
	if (bDeath)
	{
		_DEBUG("Death");
		return 0.0f;
	}

	_DEBUG("Enermy TakeDamage");

	float Damaged = 0.0f;
	float CurrentHP = UnitState.HP;
	if (CurrentHP <= DamageAmount)
	{
		Damaged = CurrentHP; // 남은 체력이 곧 라스트 데미지
		CurrentHP = 0.f;	
		UnitState.SetTakeDamageHP(CurrentHP);
		Death();
	}
	else
	{
		CurrentHP -= DamageAmount;
		Damaged = DamageAmount;
		Hit();
		UnitState.SetTakeDamageHP(CurrentHP);
	}

	Super::TakeDamage(Damaged, DamageEvent, EventInstigator, DamageCauser);

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
	// 따로 이런 함수를 만든 이유는, 쉴드 뿐만아니라 특정 상황에서 못때리는 상태 일 수도 있으므로,
	if (bBlocking)
		return false;

	if (bDeath)
		return false;

	if (bParring)
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

void AARPGEnermy_Mini::HitEnd()
{
	bHitting = false;
	bParringHit = false;
}

// 가드를 할것인지 안할 것인지 실행. 기본적으로 Guard에서 해당 함수를 실행하는 방식.
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

void AARPGEnermy_Mini::ZeroAP()
{
	Guard(false);
	EnermyAnimInstance->ZeroAP();
}

void AARPGEnermy_Mini::DeathWeaponSimulate()
{
	// 죽으면 들고있는 무기를 소켓에서 떼어내 피직스 시뮬

	FDetachmentTransformRules Rule(EDetachmentRule::KeepWorld, false);

	Weapon->DetachFromActor(Rule);
	Shield->DetachFromActor(Rule);
	Weapon->SetPhysics();
	Shield->SetPhysics();

}



// 이게 실행되었다는 것은 결국 BT에서 AttackDistance로 검사해서 문제없다는 뜻이다.
void AARPGEnermy_Mini::Attack(int32 index)
{
	if (bParringHit) // 패링당한거면 애초에 무시
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
	if (bParringHit) // 패링당한거면 애초에 무시
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
	Super::Death(); // 여기서 컨트롤러 및 모든걸 해제
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



