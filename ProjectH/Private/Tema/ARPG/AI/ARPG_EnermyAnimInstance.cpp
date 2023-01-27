// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/ARPG_EnermyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/ARPG/ARPGUnitBase.h"
#include "Tema/ARPG/ARPGEnermy_Mini.h"

UARPG_EnermyAnimInstance::UARPG_EnermyAnimInstance()
{

}

void UARPG_EnermyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerUnit = Cast<AARPGEnermy_Mini>(TryGetPawnOwner());
}

void UARPG_EnermyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (::IsValid(OwnerUnit))
	{
		if (!bDeath)
		{
			Speed = OwnerUnit->GetVelocity().Size();
			Direction = SetDirection();
			bSupArmor = OwnerUnit->bSupArmor;
			bBattleMode = OwnerUnit->bBattleMode;
			bBlocking = OwnerUnit->bBlockMode;
			bParring = OwnerUnit->bParring;
			bAttacking = OwnerUnit->bAttacking;
			bInAir = OwnerUnit->GetCharacterMovement()->IsFalling();
			bAccelerating = OwnerUnit->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0.0f;
		}

	}
}

void UARPG_EnermyAnimInstance::PlayAttackMontage(UAnimMontage* AttackMontage)
{
	Montage_Play(AttackMontage);
}

void UARPG_EnermyAnimInstance::PlayBlockingMontage()
{
	Montage_Play(BlockingMontage);
}

void UARPG_EnermyAnimInstance::PlayHitMontage(EEnermy_Mini_Mode UnitMode)
{
	switch (UnitMode)
	{
	case EEnermy_Mini_Mode::BattleMode :
		Montage_Play(HitMontage);
		break;
	case EEnermy_Mini_Mode::BlockingMode :
		Montage_Play(BlockingHitMontage);
		break;
	}
}

void UARPG_EnermyAnimInstance::PlayDeadMontage()
{
	bDeath = true;
}

void UARPG_EnermyAnimInstance::PlayParringHitMontage()
{
	Montage_Play(ParringHitMontage);
}

void UARPG_EnermyAnimInstance::ZeroAP()
{
	Montage_Play(BlockingZeroAPMontage);
}

float UARPG_EnermyAnimInstance::SetDirection()
{
	FRotator Temp;
	Temp = UKismetMathLibrary::NormalizedDeltaRotator(OwnerUnit->GetActorRotation(), OwnerUnit->GetVelocity().Rotation());

	return Temp.Yaw;
}


//-----------------------------------------------------------

void UARPG_EnermyAnimInstance::AnimNotify_AttackStart()
{
	OwnerUnit->SetWeaponCollision(true);
}

void UARPG_EnermyAnimInstance::AnimNotify_ComboSection_End()
{
	OwnerUnit->WeaponOverlapEnd();
}

void UARPG_EnermyAnimInstance::AnimNotify_Attack_End()
{
	OwnerUnit->AttackEnd();
}

void UARPG_EnermyAnimInstance::AnimNotify_ParringHit()
{
	if (ParringInstigatorUnit != nullptr)
	{
		ParringInstigatorUnit->bCanParringAttack = false; // 패링 어택 끄기
		ParringInstigatorUnit = nullptr;
		OwnerUnit->HitEnd();
	}
}

void UARPG_EnermyAnimInstance::AnimNotify_BlockStart()
{
	if(OwnerUnit)
		OwnerUnit->bBlocking = true;
}

void UARPG_EnermyAnimInstance::AnimNotify_HitEnd()
{
	OwnerUnit->HitEnd();
}

void UARPG_EnermyAnimInstance::AnimNotify_Death()
{
	OwnerUnit->DeathWeaponSimulate();
}

