// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/FstBoss/ARPG_FstBossAnimInstance.h"
#include "Tema/ARPG/AI/FstBoss/ARPGFstBoss_Projectile.h"
#include "Tema/ARPG/ARPGEnermy_FstBoss.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tema/ARPG/ARPGUnit.h"

UARPG_FstBossAnimInstance::UARPG_FstBossAnimInstance()
{

}

void UARPG_FstBossAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	FstBoss = Cast<AARPGEnermy_FstBoss>(TryGetPawnOwner());
}

void UARPG_FstBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (::IsValid(FstBoss))
	{
		if (!bDeath)
		{
			Speed = FstBoss->GetVelocity().Size();
			Direction = SetDirection();
			//bAttacking = OwnerUnit->bAttacking;
			bInAir = FstBoss->GetCharacterMovement()->IsFalling();
			bAccelerating = FstBoss->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0.0f;
		}
	}
}

float UARPG_FstBossAnimInstance::SetDirection()
{
	FRotator Temp;
	Temp = UKismetMathLibrary::NormalizedDeltaRotator(FstBoss->GetActorRotation(), FstBoss->GetVelocity().Rotation());

	return Temp.Yaw;
}


void UARPG_FstBossAnimInstance::PlayAttackMontage(UAnimMontage* AttackMontage)
{
	HittedReset(); // ��ó�� ���� ���۽ÿ��� �ʱ�ȭ �Ǿ��־���Ѵ�.

	Montage_Play(AttackMontage);
}

void UARPG_FstBossAnimInstance::PlayHitMontage()
{
	HittedReset();
	Montage_Play(HitMontage);
	if (SFXSounds.Find(ESFXMode_FstBoss::HitSound))
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode_FstBoss::HitSound].Sound, FstBoss->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode_FstBoss::HitSound].Attenuation);
}

void UARPG_FstBossAnimInstance::PlayDeadMontage()
{
	bDeath = true;
	bPlayedSound = false;
	HittedReset();
	if (SFXSounds.Find(ESFXMode_FstBoss::Death))
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode_FstBoss::Death].Sound, FstBoss->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode_FstBoss::Death].Attenuation);
}

void UARPG_FstBossAnimInstance::PlayParringHitMontage()
{
	Montage_Play(ParringHitMontage);
}

void UARPG_FstBossAnimInstance::ZeroAP()
{
	Montage_Play(ZeroAPMontage);
}


void UARPG_FstBossAnimInstance::HittedReset()
{
	CurrentEffectIndex = 0;
	CurrentSoundIndex = 0;
	CurrentAttackSoundIndex = 0;
	ProjectileCnt = 0;
}



//�޺� ������ ����ü ��Ƽ����.
void UARPG_FstBossAnimInstance::AnimNotify_ComboProjectile()
{
	if (!FstBoss && !FstBoss->BP_ComboProjectile && !FstBoss->BP_ComboProjectile_Twin)
		return;

	FTransform TF;
	FActorSpawnParameters Param;
	Param.Owner = FstBoss;
	Param.Instigator = FstBoss;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AARPGFstBoss_Projectile* Projectile = nullptr;

	switch (ProjectileCnt)
	{
	case 0:
		TF = FstBoss->GetMesh()->GetSocketTransform(FName("LeftProjectile"));
		Projectile = GetWorld()->SpawnActor<AARPGFstBoss_Projectile>(FstBoss->BP_ComboProjectile, TF, Param);
		break;
	case 1:
		TF = FstBoss->GetMesh()->GetSocketTransform(FName("RightProjectile"));
		Projectile = GetWorld()->SpawnActor<AARPGFstBoss_Projectile>(FstBoss->BP_ComboProjectile, TF, Param);
		break;
	case 2:
		TF = FstBoss->GetMesh()->GetSocketTransform(FName("TwinProjectile"));
		Projectile = GetWorld()->SpawnActor<AARPGFstBoss_Projectile>(FstBoss->BP_ComboProjectile_Twin, TF, Param);
		// ���⶧���� ����ġ������ ����. Twin ������Ÿ���� �ణ �ٸ��� ����.
		break;
	}

	++ProjectileCnt;
	if (ProjectileCnt > 2)
		ProjectileCnt = 0;

}


void UARPG_FstBossAnimInstance::AnimNotify_PlayEffect()
{
	if (!CurrentEffects.IsValidIndex(CurrentEffectIndex))
		return;

	// ����Ʈ ����
	UGameplayStatics::SpawnEmitterAttached(CurrentEffects[CurrentEffectIndex].Effect, FstBoss->GetMesh(), CurrentEffects[CurrentEffectIndex].SocketName, FVector::ZeroVector, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), EAttachLocation::SnapToTargetIncludingScale, false, EPSCPoolMethod::AutoRelease, true);
	++CurrentEffectIndex;
}


void UARPG_FstBossAnimInstance::AnimNotify_PlaySound()
{
	if (!CurrentSounds.IsValidIndex(CurrentSoundIndex))
		return;

	if (bPlayedSound)
	{
		++CurrentSoundIndex; // ���� ���嶧���� ���� �� ���� �����Ƿ� ��ĭ ����.
		return;
	}

	// ���� ���� �̹� ����ǰ��ִ� ���尡 ������ �������� �ʵ��� ����.
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CurrentSounds[CurrentSoundIndex].Sound, FstBoss->GetActorLocation(), FstBoss->GetActorRotation(), 1.f, 1.f, 1.f, CurrentSounds[CurrentSoundIndex].Attenuation);
	bPlayedSound = true;
	GetWorld()->GetTimerManager().SetTimer(SoundHandle, this, &UARPG_FstBossAnimInstance::PlayedSoundFunc, 4.f, false);
	++CurrentSoundIndex;
}


void UARPG_FstBossAnimInstance::AnimNotify_HitSound()
{
	if (SFXSounds.Find(ESFXMode_FstBoss::HitSound))
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXSounds[ESFXMode_FstBoss::HitSound].Sound, FstBoss->GetActorLocation(), 1.f, 1.f, 1.f, SFXSounds[ESFXMode_FstBoss::HitSound].Attenuation);
}


void UARPG_FstBossAnimInstance::AnimNotify_SpecialAttackEnd()
{
	if (ParringInstigatorUnit != nullptr)
	{
		ParringInstigatorUnit->bCanParringAttack = false; // �и� ���� ����
		ParringInstigatorUnit->CanSATargetUnit = nullptr;
		ParringInstigatorUnit = nullptr;
	}

	FstBoss->SpecialAttackHitEnd();
}


void UARPG_FstBossAnimInstance::AnimNotify_PlayAttackSound()
{
	if (!CurrentAttackSounds.IsValidIndex(CurrentAttackSoundIndex))
		return;

	// ���� ����
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CurrentAttackSounds[CurrentAttackSoundIndex].Sound, FstBoss->GetActorLocation(), FstBoss->GetActorRotation(), 1.f, 1.f, 1.f, CurrentAttackSounds[CurrentAttackSoundIndex].Attenuation);
	++CurrentAttackSoundIndex;
}


void UARPG_FstBossAnimInstance::PlayedSoundFunc()
{
	GetWorld()->GetTimerManager().ClearTimer(SoundHandle);
	bPlayedSound = false;
}


void UARPG_FstBossAnimInstance::AnimNotify_TwinAttackStart()
{
	if (FstBoss->bHitting)
		return;

	FstBoss->bAttacking = true;
	FstBoss->SetWeaponCollision(true, 0);
}

void UARPG_FstBossAnimInstance::AnimNotify_LeftAttackStart()
{
	if (FstBoss->bHitting)
		return;

	FstBoss->bAttacking = true;
	FstBoss->SetWeaponCollision(true, 1);
}

void UARPG_FstBossAnimInstance::AnimNotify_RightAttackStart()
{
	if (FstBoss->bHitting)
		return;

	FstBoss->bAttacking = true;
	FstBoss->SetWeaponCollision(true, 2);
}


void UARPG_FstBossAnimInstance::AnimNotify_TwinAttackEnd()
{
	FstBoss->WeaponOverlapEnd(0);
}

void UARPG_FstBossAnimInstance::AnimNotify_LeftAttackEnd()
{
	FstBoss->WeaponOverlapEnd(1);
}

void UARPG_FstBossAnimInstance::AnimNotify_RightAttackEnd()
{
	FstBoss->WeaponOverlapEnd(2);
}


void UARPG_FstBossAnimInstance::AnimNotify_Attack_End()
{
	FstBoss->AttackEnd();
}

void UARPG_FstBossAnimInstance::AnimNotify_ParringHit()
{
	if (ParringInstigatorUnit != nullptr)
	{
		ParringInstigatorUnit->bCanParringAttack = false; // �и� ���� ����
		ParringInstigatorUnit = nullptr;
		FstBoss->HitEnd();
	}
}

void UARPG_FstBossAnimInstance::AnimNotify_HitEnd()
{
	FstBoss->HitEnd();
}

void UARPG_FstBossAnimInstance::AnimNotify_Death()
{
	// ���� ���⼭ ���� �������� ���� �ùķ��̼��� ���������� �� ������ ����.
	FstBoss->DeathCollsionEnabled();

	if (DeathEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, FstBoss->GetActorTransform());

	FstBoss->SetActorHiddenInGame(true);
}


void UARPG_FstBossAnimInstance::AnimNotify_DontMoving()
{
	FstBoss->bDontMoving = true;
}


void UARPG_FstBossAnimInstance::AnimNotify_DontLockOn()
{
	FstBoss->bDontLockOn = true;
}


void UARPG_FstBossAnimInstance::AnimNotify_CanLockOn()
{
	FstBoss->bDontLockOn = false;
}


void UARPG_FstBossAnimInstance::AnimNotify_FstBoss_Teleport()
{
	FstBoss->GetMesh()->SetOnlyOwnerSee(true);
	FstBoss->GetMesh()->SetCastShadow(false);

}


void UARPG_FstBossAnimInstance::AnimNotify_FstBoss_TeleportEnd()
{

	FstBoss->GetMesh()->SetOnlyOwnerSee(false);
	FstBoss->GetMesh()->SetCastShadow(true);

	if (FstBoss->PlayerUnit)
	{
		FVector TargetLocation = FstBoss->PlayerUnit->GetActorLocation();
		TargetLocation = TargetLocation + ((FstBoss->PlayerUnit->GetActorForwardVector() * 100.f) * -1.f);
		// Ÿ���� �������� ��ġ ���.

		FstBoss->SetActorLocation(TargetLocation);
		FstBoss->SetActorRotation(FRotator(0.f, FstBoss->FindPlayerRotation().Yaw, 0.f));
	}
}



void UARPG_FstBossAnimInstance::AnimNotify_FstBoss_R_Projectile()
{
	if (!FstBoss && !FstBoss->BP_R_Projectile)
		return;

	FTransform TF;
	FActorSpawnParameters Param;
	Param.Owner = FstBoss;
	Param.Instigator = FstBoss;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AARPGFstBoss_Projectile* Projectile = nullptr;


	TF = FstBoss->GetMesh()->GetSocketTransform(FName("TwinProjectile"));
	Projectile = GetWorld()->SpawnActor<AARPGFstBoss_Projectile>(FstBoss->BP_R_Projectile, TF, Param);

}



void UARPG_FstBossAnimInstance::AnimNotify_FstBoss_E_Projectile()
{
	if (!FstBoss && !FstBoss->BP_E_Projectile)
		return;

	FTransform TF;
	FActorSpawnParameters Param;
	Param.Owner = FstBoss;
	Param.Instigator = FstBoss;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	TF = FstBoss->GetMesh()->GetSocketTransform(FName("TwinProjectile"));
	AARPGFstBoss_Projectile* Projectile = GetWorld()->SpawnActor<AARPGFstBoss_Projectile>(FstBoss->BP_E_Projectile, TF, Param);

}


void UARPG_FstBossAnimInstance::FootStepPlaySound(int32 SoundNum)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FstBoss->PhysicalSounds.Sounds[SoundNum], FstBoss->GetMesh()->GetSocketLocation(FName("Root")), 1.f, 1.f, 1.f, FstBoss->PhysicalSounds.Attenuation);
}

void UARPG_FstBossAnimInstance::AnimNotify_WalkSound()
{
	if (!FstBoss)
		return;

	if (!FstBoss->PhysicalSounds.Sounds.IsValidIndex(0))
		return;

	FootStepPlaySound(0);
}

void UARPG_FstBossAnimInstance::AnimNotify_SprintSound()
{
	if (!FstBoss)
		return;

	if (!FstBoss->PhysicalSounds.Sounds.IsValidIndex(1))
		return;

	FootStepPlaySound(1);
}


void UARPG_FstBossAnimInstance::AnimNotify_JumpSound()
{
	if (!FstBoss)
		return;

	if (!FstBoss->PhysicalSounds.Sounds.IsValidIndex(2))
		return;

	FootStepPlaySound(2);
}