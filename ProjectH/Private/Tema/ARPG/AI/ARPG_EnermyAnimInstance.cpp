// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/ARPG_EnermyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/ARPG/ARPGUnitBase.h"
#include "Tema/ARPG/ARPGEnermy_Mini.h"
#include "Kismet/GameplayStatics.h"

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
	CurrentEffectIndex = 0;
	CurrentSoundIndex = 0;
	CurrentAttackSoundIndex = 0;
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
		if (SFXSounds.Find(ESFXMode_Enermy_Mini::HitSound))
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode_Enermy_Mini::HitSound].Sound, OwnerUnit->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode_Enermy_Mini::HitSound].Attenuation);
		break;
	case EEnermy_Mini_Mode::BlockingMode :
		Montage_Play(BlockingHitMontage);
		if (SFXSounds.Find(ESFXMode_Enermy_Mini::BlockingHitSound))
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode_Enermy_Mini::BlockingHitSound].Sound, OwnerUnit->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode_Enermy_Mini::BlockingHitSound].Attenuation);
		break;
	}
}

void UARPG_EnermyAnimInstance::PlayDeadMontage()
{
	bDeath = true;
	if (SFXSounds.Find(ESFXMode_Enermy_Mini::Death))
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode_Enermy_Mini::Death].Sound, OwnerUnit->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode_Enermy_Mini::Death].Attenuation);
}

void UARPG_EnermyAnimInstance::PlayParringHitMontage()
{
	Montage_Play(ParringHitMontage);
}

void UARPG_EnermyAnimInstance::ZeroAP()
{
	//bBlocking을 조사해서 블록킹중이면 블록 Zero 아니면 그로기 몽타주 실행
	Montage_Play(BlockingZeroAPMontage);
}

void UARPG_EnermyAnimInstance::SpecialAttackHitMontagePlay()
{
	Montage_Play(SpecialAttackHitMontage);
}

float UARPG_EnermyAnimInstance::SetDirection()
{
	FRotator Temp;
	Temp = UKismetMathLibrary::NormalizedDeltaRotator(OwnerUnit->GetActorRotation(), OwnerUnit->GetVelocity().Rotation());

	return Temp.Yaw;
}


//-----------------------------------------------------------

void UARPG_EnermyAnimInstance::AnimNotify_PlayEffect()
{
	if (!CurrentEffects.IsValidIndex(CurrentEffectIndex))
		return;

	// 이펙트 실행
	UGameplayStatics::SpawnEmitterAttached(CurrentEffects[CurrentEffectIndex].Effect, OwnerUnit->GetMesh(), CurrentEffects[CurrentEffectIndex].SocketName, FVector::ZeroVector, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), EAttachLocation::SnapToTargetIncludingScale, false, EPSCPoolMethod::AutoRelease, true);
	++CurrentEffectIndex;
}

void UARPG_EnermyAnimInstance::AnimNotify_PlaySound()
{
	if (!CurrentSounds.IsValidIndex(CurrentSoundIndex))
		return;

	// 사운드 실행
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CurrentSounds[CurrentSoundIndex].Sound, OwnerUnit->GetActorLocation(), OwnerUnit->GetActorRotation(), 1.f, 1.f, 1.f, CurrentSounds[CurrentSoundIndex].Attenuation);

	bPlayedSound = true;
	GetWorld()->GetTimerManager().SetTimer(SoundHandle, this, &UARPG_EnermyAnimInstance::PlayedSoundFunc, 5.f, false);

	++CurrentSoundIndex;
}


void UARPG_EnermyAnimInstance::PlayedSoundFunc()
{
	GetWorld()->GetTimerManager().ClearTimer(SoundHandle);
	bPlayedSound = false;
}

void UARPG_EnermyAnimInstance::AnimNotify_PlayAttackSound()
{
	if (!CurrentAttackSounds.IsValidIndex(CurrentAttackSoundIndex))
		return;

	// 사운드 실행
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CurrentAttackSounds[CurrentAttackSoundIndex].Sound, OwnerUnit->GetActorLocation(), OwnerUnit->GetActorRotation(), 1.f, 1.f, 1.f, CurrentAttackSounds[CurrentAttackSoundIndex].Attenuation);
	++CurrentAttackSoundIndex;
}

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
	OwnerUnit->DeathCollsionEnabled(); 
	// 바로 콜리전을 해제해버리면 땅에 들어가는등 이상한 행동을 하므로 애니를 기다림.
}

void UARPG_EnermyAnimInstance::AnimNotify_DontMoving()
{
	OwnerUnit->bDontMoving = true;
}


void UARPG_EnermyAnimInstance::AnimNotify_DontLockOn()
{
	OwnerUnit->bDontLockOn = true;
}



void UARPG_EnermyAnimInstance::AnimNotify_CanLockOn()
{
	OwnerUnit->bDontLockOn = false;
}

void UARPG_EnermyAnimInstance::AnimNotify_HitSound()
{
	if (SFXSounds.Find(ESFXMode_Enermy_Mini::HitSound))
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode_Enermy_Mini::HitSound].Sound, OwnerUnit->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode_Enermy_Mini::HitSound].Attenuation);
}


void UARPG_EnermyAnimInstance::FootStepPlaySound(int32 SoundNum)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), OwnerUnit->PhysicalSounds.Sounds[SoundNum], OwnerUnit->GetMesh()->GetSocketLocation(FName("Root")), 1.f, 1.f, 1.f, OwnerUnit->PhysicalSounds.Attenuation);
}

void UARPG_EnermyAnimInstance::AnimNotify_WalkSound()
{
	if (!OwnerUnit->PhysicalSounds.Sounds.IsValidIndex(0))
		return;

	FootStepPlaySound(0);
}

void UARPG_EnermyAnimInstance::AnimNotify_SprintSound()
{
	if (!OwnerUnit->PhysicalSounds.Sounds.IsValidIndex(1))
		return;

	FootStepPlaySound(1);
}


void UARPG_EnermyAnimInstance::AnimNotify_JumpSound()
{
	if (!OwnerUnit->PhysicalSounds.Sounds.IsValidIndex(2))
		return;

	FootStepPlaySound(2);
}



void UARPG_EnermyAnimInstance::AnimNotify_SpecialAttackEnd()
{
	if (ParringInstigatorUnit != nullptr)
	{
		ParringInstigatorUnit->bCanParringAttack = false; // 패링 어택 끄기
		ParringInstigatorUnit->CanSATargetUnit = nullptr;
		ParringInstigatorUnit = nullptr;
	}

	OwnerUnit->SpecialAttackHitEnd();
}