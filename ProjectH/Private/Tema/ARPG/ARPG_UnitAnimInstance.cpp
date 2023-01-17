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
		if (!bDeath)
		{
			bIsInAir = OwnerUnit->GetCharacterMovement()->IsFalling();
			IsMoving = OwnerUnit->GetVelocity().Length() > 0.f;
			bIsSprint = OwnerUnit->bSprint;
			bIsBlocking = OwnerUnit->bBlockMode;
			//bHitting = OwnerUnit->bHitting;

			bIsAttacking = OwnerUnit->bAttacking;
			AttackingLeft = OwnerUnit->bAttackLeft;
			AttackingRight = OwnerUnit->bAttackRight;
			AttackingForward = OwnerUnit->bAttackForward;
			AttackingBackward = OwnerUnit->bAttackBackward;
		}

	}
}

void UARPG_UnitAnimInstance::Parring()
{
	OwnerUnit->bParringPlaying = true;
	OwnerUnit->bBlocking = false;
	OwnerUnit->bBlockMode = false;
	Montage_Play(ParringMontage);
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
	bDeath = true;
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


void UARPG_UnitAnimInstance::ShieldZeroAP()
{
	Montage_Play(BlockingZeroAPMontage);
	OwnerUnit->ZeroAP();
}


void UARPG_UnitAnimInstance::ParringAttack()
{
	Montage_Play(ParringAttackMontage);
}


//----------------------------------
// 노티파이

void UARPG_UnitAnimInstance::AnimNotify_BattleMode()
{
	if (!OwnerUnit)
		return;

	if (OwnerUnit->FPSWeapon)
	{
		OwnerUnit->FPSWeapon->AttachToComponent(OwnerUnit->FPSMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, OwnerUnit->WeaponSocketName);
	}
}

void UARPG_UnitAnimInstance::AnimNotify_NormalMode()
{
	if (!OwnerUnit)
		return;

	if (OwnerUnit->FPSWeapon)
	{
		OwnerUnit->FPSWeapon->AttachToComponent(OwnerUnit->FPSMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, OwnerUnit->IdleSocketName);
	}
}

void UARPG_UnitAnimInstance::AnimNotify_AttackStart()
{
	if (OwnerUnit)
	{
		float AP = OwnerUnit->UnitState.AP - OwnerUnit->LMB_AP;
		OwnerUnit->UnitState.SetAP(AP);
		OwnerUnit->bUseAP = true;
		OwnerUnit->bUsingAP = false;
		OwnerUnit->SetWeaponCollision(true);
	}
}

void UARPG_UnitAnimInstance::AnimNotify_ComboSection_End()
{
	if (OwnerUnit)
	{
		//OwnerUnit->OnEndAP.Broadcast();
		//이 노티파이는 그냥 연속 공격 몽타주를 위한 콜리전 껏다 켜기라서 하나의 동작이니 실행x

		OwnerUnit->WeaponOverlapEnd();
	}
}

void UARPG_UnitAnimInstance::AnimNotify_Attack_End()
{
	if (OwnerUnit)
	{
		OwnerUnit->OnEndAP.Broadcast();
		//OwnerUnit->AttackEnd(); 하지 않는 이유는 ARPGUnit의 AttackEnd를 참조. ★★
	}
}

void UARPG_UnitAnimInstance::AnimNotify_BlockStart()
{
	if (OwnerUnit)
	{
		OwnerUnit->bBlocking = true;
		OwnerUnit->GetCharacterMovement()->MaxWalkSpeed = OwnerUnit->BlockSpeed;
	}
}

void UARPG_UnitAnimInstance::AnimNotify_HitEnd()
{
	if (OwnerUnit)
	{
		OwnerUnit->OnEndAP.Broadcast();
		OwnerUnit->HitEnd();
		//_DEBUG("Hit End");
	}
}

void UARPG_UnitAnimInstance::AnimNotify_ParringStart()
{
	OwnerUnit->bParring = true;
}

void UARPG_UnitAnimInstance::AnimNotify_ParringEnd()
{	
	OwnerUnit->bParring = false;
	OwnerUnit->OnEndAP.Broadcast();
	if (OwnerUnit->bRMBPush)
	{
		OwnerUnit->bBlockMode = true;
	}
}


void UARPG_UnitAnimInstance::AnimNotify_ParringPlayingEnd()
{
	// 패링 중이니 패링을 다시 못누르게 하는 것.
	OwnerUnit->bParringPlaying = false;

	// 패링을하고 공격하기엔 너무 빠르다. 
	// 때문에 적이 패링 히트 몽타주실행이 끝날때 false하기로함.
	//OwnerUnit->bCanParringAttack = false;
	
}

void UARPG_UnitAnimInstance::AnimNotify_SprintStart()
{
	// 달리기 애님이 무한 반복이기때문에, 혹여나 달리기가아닌데 이 노티파이가 실행되면,
	// 달리기를 누르지 않았음에도 불구하고 AP가 빠지는 기이한 현상을 방지하기 위해
	// 지금 달리기 키가 눌려있는지 판단.

	if (bIsSprint) 
	{
		OwnerUnit->OnUsingAP.Broadcast();
		OwnerUnit->GetCharacterMovement()->MaxWalkSpeed = OwnerUnit->WalkSpeed + OwnerUnit->SprintSpeed;
	}
}

void UARPG_UnitAnimInstance::AnimNotify_UseAP()
{
	OwnerUnit->OnUseAP.Broadcast();
}

void UARPG_UnitAnimInstance::AnimNotify_HoldAP()
{
	OwnerUnit->OnUsingAP.Broadcast();
}

void UARPG_UnitAnimInstance::AnimNotify_HoldAttack()
{
	if (OwnerUnit->bChargeAttacking == false)
	{
		OwnerUnit->ChargeAttackStart();
	}
}

void UARPG_UnitAnimInstance::AnimNotify_SpecialAttackEnd()
{
	OwnerUnit->bSpecialAttackMode = false;
	OwnerUnit->bSpecialAttackPlaying = false;
	OwnerUnit->bCanParringAttack = false;
	OwnerUnit->OnEndAP.Broadcast();
	OwnerUnit->UnitState.ATK = OwnerUnit->UnitState.NormallyATK;
}

void UARPG_UnitAnimInstance::AnimNotify_Death()
{
	OwnerUnit->DeathWeaponSimulate();
}
