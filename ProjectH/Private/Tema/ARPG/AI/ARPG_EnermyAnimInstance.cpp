// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/ARPG_EnermyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/ARPG/ARPGEnermy.h"

UARPG_EnermyAnimInstance::UARPG_EnermyAnimInstance()
{

}

void UARPG_EnermyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	OwnerPawn = TryGetPawnOwner();
	if (::IsValid(OwnerPawn))
	{
		Speed = OwnerPawn->GetVelocity().Size();
		Direction = SetDircection(OwnerPawn);

		if (CurrentCharacter)
		{
			bInAir = CurrentCharacter->IsFalling();
		}
	}
}

float UARPG_EnermyAnimInstance::SetDircection(APawn* Pawn)
{
	if (::IsValid(Pawn))
	{
		FRotator Temp;
		Temp = UKismetMathLibrary::NormalizedDeltaRotator(Pawn->GetActorRotation(), Pawn->GetVelocity().Rotation());

		return Temp.Yaw * -1.f;
	}

	return 0.0f;
}

