// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPG_TPSAnimInstance.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Kismet/KismetMathLibrary.h"

UARPG_TPSAnimInstance::UARPG_TPSAnimInstance()
{
	bIsSheathed = true;
}

void UARPG_TPSAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerUnit = Cast<AARPGUnit>(TryGetPawnOwner());
}

void UARPG_TPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (::IsValid(OwnerUnit))
	{
		if (!bDeath)
		{
			bIsInAir = OwnerUnit->GetCharacterMovement()->IsFalling();
			IsMoving = OwnerUnit->GetVelocity().Length() > 0.f;
			bIsSprint = OwnerUnit->bSprint;
			bIsBlocking = OwnerUnit->bBlockMode;
			Direction = GetDirection();

			bIsAttacking = OwnerUnit->bAttacking;
			AttackingLeft = OwnerUnit->bAttackLeft;
			AttackingRight = OwnerUnit->bAttackRight;
			AttackingForward = OwnerUnit->bAttackForward;
			AttackingBackward = OwnerUnit->bAttackBackward;
		}

	}
}

void UARPG_TPSAnimInstance::Parring()
{
	/*OwnerUnit->bParringPlaying = true;
	OwnerUnit->bBlocking = false;
	OwnerUnit->bBlockMode = false;*/
	Montage_Play(ParringMontage);
}

void UARPG_TPSAnimInstance::Hit(EUnitMode UnitMode)
{
	switch (UnitMode)
	{
	case EUnitMode::BattleMode:
		Montage_Play(HitMontage);
		break;
	case EUnitMode::BlockingMode:
		Montage_Play(BlockingHitMontage);
		break;
	}
}

void UARPG_TPSAnimInstance::Death()
{
	bDeath = true;
}

void UARPG_TPSAnimInstance::WeaponOnOff(bool bFlag)
{
	if (bFlag)
	{
		Montage_Play(WeaponCloseMontage);
	}
	else
	{
		Montage_Play(WeaponOpenMontage);
	}
}


void UARPG_TPSAnimInstance::ShieldZeroAP()
{
	Montage_Play(BlockingZeroAPMontage);
	//OwnerUnit->ZeroAP();
}


void UARPG_TPSAnimInstance::ParringAttack()
{
	Montage_Play(ParringAttackMontage);
}

float UARPG_TPSAnimInstance::GetDirection()
{
	FRotator Temp;
	Temp = UKismetMathLibrary::NormalizedDeltaRotator(OwnerUnit->GetActorRotation(), OwnerUnit->GetVelocity().Rotation());

	return Temp.Yaw;
}


//----------------------------------
// 노티파이

void UARPG_TPSAnimInstance::AnimNotify_BattleMode()
{
	if (!OwnerUnit)
		return;

	/*if (OwnerUnit->TPSWeapon)
	{
		OwnerUnit->TPSWeapon->AttachToComponent(OwnerUnit->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, OwnerUnit->WeaponSocketName);
		bIsSheathed = false;
	}*/
}

void UARPG_TPSAnimInstance::AnimNotify_NormalMode()
{
	if (!OwnerUnit)
		return;

	/*if (OwnerUnit->TPSWeapon)
	{
		OwnerUnit->TPSWeapon->AttachToComponent(OwnerUnit->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, OwnerUnit->IdleSocketName);
		bIsSheathed = true;
	}*/
}
