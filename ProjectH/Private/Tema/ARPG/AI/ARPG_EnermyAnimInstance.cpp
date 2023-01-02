// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/ARPG_EnermyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/ARPG/ARPGEnermy_Mini.h"

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
		Speed = OwnerUnit->GetVelocity().Size();
		Direction = SetDircection();

		bBattleMode = OwnerUnit->bBattleMode;
		bBlocking = OwnerUnit->bBlocking;
		bParring = OwnerUnit->bParring;
		bHitting = OwnerUnit->bHitting;
		bAttacking = OwnerUnit->bAttacking;
		bInAir = OwnerUnit->GetCharacterMovement()->IsFalling();
		bAccelerating = OwnerUnit->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0.0f;
	}
}

void UARPG_EnermyAnimInstance::PlayAttackMontage(UAnimMontage* AttackMontage)
{
	Montage_Play(AttackMontage);
}

void UARPG_EnermyAnimInstance::PlayHitMontage()
{
	Montage_Play(HitMontage);
}

void UARPG_EnermyAnimInstance::PlayDeadMontage()
{
	Montage_Play(DeadMontage);
}

float UARPG_EnermyAnimInstance::SetDircection()
{
	
	FRotator Temp;
	Temp = UKismetMathLibrary::NormalizedDeltaRotator(OwnerUnit->GetActorRotation(), OwnerUnit->GetVelocity().Rotation());

	return Temp.Yaw;
}

