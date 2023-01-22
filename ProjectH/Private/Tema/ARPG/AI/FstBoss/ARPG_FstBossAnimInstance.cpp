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
