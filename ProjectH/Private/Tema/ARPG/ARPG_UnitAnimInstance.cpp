// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPG_UnitAnimInstance.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Kismet/GameplayStatics.h"

UARPG_UnitAnimInstance::UARPG_UnitAnimInstance()
{
	bIsSheathed = true;
}

void UARPG_UnitAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OnMontageEnded.AddDynamic(this, &UARPG_UnitAnimInstance::MontageEnd);
	OnMontageStarted.AddDynamic(this, &UARPG_UnitAnimInstance::MontageStarted);

	OwnerUnit = Cast<AARPGUnit>(TryGetPawnOwner());
}


void UARPG_UnitAnimInstance::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bMontagePlaying = false;
}

void UARPG_UnitAnimInstance::MontageStarted(UAnimMontage* Montage)
{
	bMontagePlaying = true;
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
		if (SFXSounds.Find(ESFXMode::HitSound))
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode::HitSound].Sound, OwnerUnit->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode::HitSound].Attenuation);
		
		break;
	case EUnitMode::BlockingMode :
		Montage_Play(BlockingHitMontage);
		if (SFXSounds.Find(ESFXMode::BlockingHitSound))
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode::BlockingHitSound].Sound, OwnerUnit->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode::BlockingHitSound].Attenuation);
		break;
	}

	
}

void UARPG_UnitAnimInstance::Death()
{
	bDeath = true;
	if (SFXSounds.Find(ESFXMode::Death))
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode::Death].Sound, OwnerUnit->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode::Death].Attenuation);
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

void UARPG_UnitAnimInstance::AnimNotify_PlayEffect()
{
}

void UARPG_UnitAnimInstance::AnimNotify_PlaySound()
{
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
		bIsSheathed = false;
		OwnerUnit->OnWeaponDraw.Execute();
		if(SFXSounds.Find(ESFXMode::DrawSword))
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode::DrawSword].Sound, OwnerUnit->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode::DrawSword].Attenuation);
	}
}

void UARPG_UnitAnimInstance::AnimNotify_NormalMode()
{
	if (!OwnerUnit)
		return;

	if (OwnerUnit->FPSWeapon)
	{
		OwnerUnit->FPSWeapon->AttachToComponent(OwnerUnit->FPSMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, OwnerUnit->IdleSocketName);
		bIsSheathed = true;
		OwnerUnit->OnWeaponStow.Execute();
		if (SFXSounds.Find(ESFXMode::StowSword))
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode::StowSword].Sound, OwnerUnit->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode::StowSword].Attenuation);
	
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
		OwnerUnit->OnAttackAP.Broadcast(); // AP 깎는 연출
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
		OwnerUnit->AttackEnd();
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
	}
}

void UARPG_UnitAnimInstance::AnimNotify_ParringStart()
{
	OwnerUnit->bParring = true;
	OwnerUnit->ParringSwingSFX();
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


void UARPG_UnitAnimInstance::FootStepPlaySound(int32 SoundNum)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), OwnerUnit->PhysicalSounds.Sounds[SoundNum], OwnerUnit->GetMesh()->GetSocketLocation(FName("Root")), 1.f, 1.f, 1.f, OwnerUnit->PhysicalSounds.Attenuation);
}

void UARPG_UnitAnimInstance::AnimNotify_WalkSound()
{
	if (!OwnerUnit->PhysicalSounds.Sounds.IsValidIndex(0))
		return;

	FootStepPlaySound(0);
}

void UARPG_UnitAnimInstance::AnimNotify_SprintSound()
{
	if (!OwnerUnit->PhysicalSounds.Sounds.IsValidIndex(1))
		return;

	FootStepPlaySound(1);
}


void UARPG_UnitAnimInstance::AnimNotify_JumpSound()
{
	if (!OwnerUnit->PhysicalSounds.Sounds.IsValidIndex(2))
		return;

	FootStepPlaySound(2);
}