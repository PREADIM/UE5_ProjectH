// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectHAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/ProjectHCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


UProjectHAnimInstance::UProjectHAnimInstance()
{
	bIsMoving = false;
	bIsRunning = false;
}

void UProjectHAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerCharacter = Cast<class AProjectHCharacter>(GetOwningActor());
	if (OwnerCharacter)
	{
		MovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UProjectHAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (::IsValid(OwnerCharacter))
	{
		CurrentSpeed = OwnerCharacter->GetVelocity().Size();
		bIsMoving = bMoving();
		Direction = SetDirection();
		bIsInAir = MovementComponent->IsFalling();
	}
}


bool UProjectHAnimInstance::bMoving()
{
	CurrentAccel = MovementComponent->GetCurrentAcceleration().Size();
	return MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector && CurrentSpeed > 10.0f;
}


float UProjectHAnimInstance::SetDirection()
{
	FRotator Temp;
	Temp = UKismetMathLibrary::NormalizedDeltaRotator(OwnerCharacter->GetActorRotation(), OwnerCharacter->GetVelocity().Rotation());

	return Temp.Yaw;
}


void UProjectHAnimInstance::AnimNotify_Disable()
{
	OwnerCharacter->bPlay = false;
}

void UProjectHAnimInstance::AnimNotify_Enable()
{
	OwnerCharacter->bPlay = true;
}