// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPG_UnitAnimInstance.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"

UARPG_UnitAnimInstance::UARPG_UnitAnimInstance()
{
	bIsSheathed = true;
}

void UARPG_UnitAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerUnit = Cast<AARPGUnit>(TryGetPawnOwner());
}

void UARPG_UnitAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (::IsValid(OwnerUnit))
	{
		bIsInAir = OwnerUnit->GetCharacterMovement()->IsFalling();
		IsMoving = OwnerUnit->GetVelocity().Length() > 0.f;
		bIsSprint = OwnerUnit->bSprint;
		bIsParring = OwnerUnit->bParring;
		bIsBlocking = OwnerUnit->bBlockMode;
		bHitting = OwnerUnit->bHitting;

		bIsAttacking = OwnerUnit->bAttacking;
		AttackingLeft = OwnerUnit->bAttackLeft;
		AttackingRight = OwnerUnit->bAttackRight;
		AttackingForward = OwnerUnit->bAttackForward;
		AttackingBackward = OwnerUnit->bAttackBackward;
	}
}

void UARPG_UnitAnimInstance::Hit(EUnitMode UnitMode)
{
	switch (UnitMode)
	{
	case EUnitMode::BattleMode :
		Montage_Play(HitMontage);
		break;
	case EUnitMode::BlockingMode :
		Montage_Play(BlockingHitMontage);
		break;
	}	
}

void UARPG_UnitAnimInstance::Death()
{
	Montage_Play(DeathMontage);
}

void UARPG_UnitAnimInstance::WeaponOnOff(bool bFlag)
{
	if (bFlag)
	{
		Montage_Play(WeaponCloseMontage);
	}
	else
	{
		Montage_Play(WeaponOpenMontage);
	}

	bIsSheathed = bFlag;
}


void UARPG_UnitAnimInstance::ZeroAP()
{
	Montage_Play(BlockingZeroAPMontage);
}


//----------------------------------
// 노티파이

void UARPG_UnitAnimInstance::AnimNotify_BattleMode()
{
	if (!OwnerUnit)
		return;

	if (OwnerUnit->Weapon)
	{
		OwnerUnit->Weapon->AttachToComponent(OwnerUnit->FPSMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, OwnerUnit->WeaponSocketName);
	}
}

void UARPG_UnitAnimInstance::AnimNotify_NormalMode()
{
	if (!OwnerUnit)
		return;

	if (OwnerUnit->Weapon)
	{
		OwnerUnit->Weapon->AttachToComponent(OwnerUnit->FPSMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, OwnerUnit->IdleSocketName);
	}
}

void UARPG_UnitAnimInstance::AnimNotify_AttackStart()
{
	if (OwnerUnit)
	{
		OwnerUnit->SetWeaponCollision(true);
	}
}

void UARPG_UnitAnimInstance::AnimNotify_ComboSection_End()
{
	if (OwnerUnit)
	{
		OwnerUnit->WeaponOverlapEnd();
	}
}

void UARPG_UnitAnimInstance::AnimNotify_Attack_End()
{
	if (OwnerUnit)
	{
		OwnerUnit->AttackEnd();
	}
}

void UARPG_UnitAnimInstance::AnimNotify_BlockStart()
{
	if (OwnerUnit)
	{
		OwnerUnit->bBlocking = true;
	}
}

void UARPG_UnitAnimInstance::AnimNotify_HitEnd()
{
	if (OwnerUnit)
	{
		OwnerUnit->HitEnd();
		_DEBUG("Hit End");
	}
}

void UARPG_UnitAnimInstance::AnimNotify_ParringStart()
{
	OwnerUnit->SetShieldCollision(true);
	OwnerUnit->bParring = true;
}

void UARPG_UnitAnimInstance::AnimNotify_ParringEnd()
{
	if (OwnerUnit)
	{
		OwnerUnit->SetShieldCollision(false);
		OwnerUnit->bParring = false;
	}
}

