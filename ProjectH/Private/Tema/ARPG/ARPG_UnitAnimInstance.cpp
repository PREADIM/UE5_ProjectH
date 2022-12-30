// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPG_UnitAnimInstance.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/ARPG/ARPGWeapon.h"
#include "Tema/ARPG/ARPGShield.h"

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
		bIsBlocking = OwnerUnit->bBlocking;
		bShieldHit = OwnerUnit->bShieldHit;

		bIsAttacking = OwnerUnit->bAttacking;
		AttackingLeft = OwnerUnit->bAttackLeft;
		AttackingRight = OwnerUnit->bAttackRight;
		AttackingForward = OwnerUnit->bAttackForward;
		AttackingBackward = OwnerUnit->bAttackBackward;
	}

}

void UARPG_UnitAnimInstance::Hit()
{
	Montage_Play(HitMontage);
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

void UARPG_UnitAnimInstance::AnimNotify_Attack_End()
{
	if (OwnerUnit)
	{
		OwnerUnit->SetWeaponCollision(false);
		OwnerUnit->AttackEnd();
	}
}

void UARPG_UnitAnimInstance::AnimNotify_ParringEnd()
{
	if (OwnerUnit)
	{
		OwnerUnit->bParring = false;
		bIsParring = false;
	}
}



//----------------------------------
