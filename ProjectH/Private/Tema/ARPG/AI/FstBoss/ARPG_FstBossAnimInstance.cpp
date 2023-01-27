// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/FstBoss/ARPG_FstBossAnimInstance.h"
#include "Tema/ARPG/ARPGEnermy_FstBoss.h"
#include "Kismet/KismetMathLibrary.h"

UARPG_FstBossAnimInstance::UARPG_FstBossAnimInstance()
{

}

void UARPG_FstBossAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	FstBoss = Cast<AARPGEnermy_FstBoss>(FstBoss);
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
			bSupArmor = FstBoss->bSupArmor;
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


void UARPG_FstBossAnimInstance::AnimNotify_PlayEffect()
{

}


void UARPG_FstBossAnimInstance::AnimNotify_PlaySound()
{

}


void UARPG_FstBossAnimInstance::PlayAttackMontage(UAnimMontage* AttackMontage)
{
	Montage_Play(AttackMontage);
}

void UARPG_FstBossAnimInstance::PlayHitMontage()
{
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

void UARPG_FstBossAnimInstance::AnimNotify_AttackStart()
{
	FstBoss->SetWeaponCollision(true);
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


