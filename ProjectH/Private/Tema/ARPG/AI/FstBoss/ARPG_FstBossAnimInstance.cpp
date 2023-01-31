// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/FstBoss/ARPG_FstBossAnimInstance.h"
#include "Tema/ARPG/AI/FstBoss/ARPGFstBoss_Projectile.h"
#include "Tema/ARPG/ARPGEnermy_FstBoss.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	HittedReset(); // 어처피 공격 시작시에는 초기화 되어있어야한다.

	Montage_Play(AttackMontage);
}

void UARPG_FstBossAnimInstance::PlayHitMontage()
{
	HittedReset();
	Montage_Play(HitMontage);
}

void UARPG_FstBossAnimInstance::PlayDeadMontage()
{
	bDeath = true;
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
	ProjectileCnt = 0;
}



//콤보 어택의 투사체 노티파이.
void UARPG_FstBossAnimInstance::AnimNotify_ComboProjectile()
{
	if (!FstBoss && !FstBoss->BP_ComboProjectile && !FstBoss->BP_ComboProjectile_Twin)
		return;

	FTransform TF;
	FActorSpawnParameters Param;
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
		// 여기때문에 스위치문에서 실행. Twin 프로젝타일은 약간 다르게 생김.
		break;
	}

	if (Projectile)
	{
		_DEBUG("Projectile");
		//Projectile->OwnerUnit = FstBoss;
	}


	++ProjectileCnt;
	if (ProjectileCnt > 2)
		ProjectileCnt = 0;

}


void UARPG_FstBossAnimInstance::AnimNotify_PlayEffect()
{
	if (!CurrentEffects.IsValidIndex(CurrentEffectIndex))
		return;

	// 이펙트 실행
	UGameplayStatics::SpawnEmitterAttached(CurrentEffects[CurrentEffectIndex].Effect, FstBoss->GetMesh(), CurrentEffects[CurrentEffectIndex].SocketName);
	++CurrentEffectIndex;
}


void UARPG_FstBossAnimInstance::AnimNotify_PlaySound()
{
	if (!CurrentSounds.IsValidIndex(CurrentSoundIndex))
		return;

	// 사운드 실행
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CurrentSounds[CurrentSoundIndex], FstBoss->GetActorLocation(), FstBoss->GetActorRotation());
	++CurrentSoundIndex;
}


void UARPG_FstBossAnimInstance::AnimNotify_TwinAttackStart()
{
	FstBoss->SetWeaponCollision(true, 0);
}

void UARPG_FstBossAnimInstance::AnimNotify_LeftAttackStart()
{
	FstBoss->SetWeaponCollision(true, 1);
}

void UARPG_FstBossAnimInstance::AnimNotify_RightAttackStart()
{
	FstBoss->SetWeaponCollision(true, 2);
}

void UARPG_FstBossAnimInstance::AnimNotify_ComboSection_End()
{
	FstBoss->WeaponOverlapEnd();
}

void UARPG_FstBossAnimInstance::AnimNotify_Attack_End()
{
	FstBoss->AttackEnd();
}

void UARPG_FstBossAnimInstance::AnimNotify_ParringHit()
{
	if (ParringInstigatorUnit != nullptr)
	{
		ParringInstigatorUnit->bCanParringAttack = false; // 패링 어택 끄기
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
	// 보통 여기서 무기 피직스를 끄는 시뮬레이션을 진행하지만 이 보스는 없다.
}


