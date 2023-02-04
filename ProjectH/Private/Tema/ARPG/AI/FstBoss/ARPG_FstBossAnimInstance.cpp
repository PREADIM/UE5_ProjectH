// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/FstBoss/ARPG_FstBossAnimInstance.h"
#include "Tema/ARPG/AI/FstBoss/ARPGFstBoss_Projectile.h"
#include "Tema/ARPG/ARPGEnermy_FstBoss.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
//#include "NavigationSystem.h"
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
	UGameplayStatics::SpawnEmitterAttached(CurrentEffects[CurrentEffectIndex].Effect, FstBoss->GetMesh(), CurrentEffects[CurrentEffectIndex].SocketName);
	++CurrentEffectIndex;
}


void UARPG_FstBossAnimInstance::AnimNotify_PlaySound()
{
	if (!CurrentSounds.IsValidIndex(CurrentSoundIndex))
		return;

	// ���� ����
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CurrentSounds[CurrentSoundIndex], FstBoss->GetActorLocation(), FstBoss->GetActorRotation());
	++CurrentSoundIndex;
}


void UARPG_FstBossAnimInstance::AnimNotify_TwinAttackStart()
{
	FstBoss->bAttacking = true;
	FstBoss->SetWeaponCollision(true, 0);
}

void UARPG_FstBossAnimInstance::AnimNotify_LeftAttackStart()
{
	FstBoss->bAttacking = true;
	FstBoss->SetWeaponCollision(true, 1);
}

void UARPG_FstBossAnimInstance::AnimNotify_RightAttackStart()
{
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
}


void UARPG_FstBossAnimInstance::AnimNotify_FstBoss_TeleportEnd()
{

	FstBoss->GetMesh()->SetOnlyOwnerSee(false);

	if (FstBoss->PlayerUnit)
	{
		_DEBUG("TeleportPlayerUnit");
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