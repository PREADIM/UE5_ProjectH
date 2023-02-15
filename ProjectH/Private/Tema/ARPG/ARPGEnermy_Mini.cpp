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

	if (GetMesh()) // 가끔 C++ 클래스를 상속받은 블프가 디테일창이 사라지면서 null값을 넣기때문.(버그)
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

	//무기 강인도를 이용해서 맞은 액터의 강인도를 깎는다.
	Super::TakeDamageCalculator(DamageWeapon, DamageEvent, EventInstigator, DamageCauser);

	if (bBlocking)
	{
		if (TargetDotProduct(DamageCauser->GetActorLocation(), 0.7)) // 45도 가량
		{
			float APDMG = DamageCauser->CalculAPDamage(DamageWeapon->WeaponAP_DMG);
			Damaged = Damaged - (Damaged * BlockingDEF); // BlockingDEF는 0.0~1.0으로 되어있다.
			TakeDamageAP(APDMG);
			bBlockingHit = true;
		}
	}


	if (CurrentHP <= Damaged)
	{
		Damaged = CurrentHP; // 남은 체력이 곧 라스트 데미지
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
		bool bHitMontagePlay = Super::Hit(bBlockingHit); // 강인도 검사
		// 적의 경우 AP는 그로기 여부이고, 강인도는 슈퍼아머냐 아니냐의 차이이다.

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


// 배틀모드가 실행 되었을때 실행되는 함수. true면 배틀 시작시 버프라던가 무언가 실행 가능.
// 해당 클래스는 그런거 없는 기본 적이므로 그냥 배틀모드끝나면 가드중인거 풀기 정도.
void AARPGEnermy_Mini::SetBattleMode(bool bFlag)
{
	if (bBattleMode == bFlag) // 이게 true면 이미 한번이상 실행 되었다는 뜻
		return;

	// bBattleMode 값을 바꿔주면서 동시에 강인도 초기화.
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

//에너미의 ZeroAP의 경우는 그로기 상태를 말한다.
void AARPGEnermy_Mini::ZeroAP()
{
	Super::ZeroAP();
	Guard(false);
	EnermyAnimInstance->ZeroAP();
}

void AARPGEnermy_Mini::DeathWeaponSimulate()
{
	// 죽으면 들고있는 무기를 소켓에서 떼어내 피직스 시뮬

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
	DeathReset();
	Super::Death(); // 여기서 컨트롤러 및 모든걸 해제	
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

// 몽타주 끝나면 하면 될듯.
void AARPGEnermy_Mini::AttackEnd()
{
	bAttacking = false;
	bDontLockOn = false;

	if (!bHitting) // bHitting이 true면 HitEnd에서 맞은 애니메이션이 끝난후 false.
		bDontMoving = false;

	OnAttack.Broadcast();
	OnAttack.Clear();
}

void AARPGEnermy_Mini::WeaponOverlapEnd()
{
	SetWeaponCollision(false);
	Weapon->AttackEnd();
}



