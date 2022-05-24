// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectHAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/ProjectHCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


UProjectHAnimInstance::UProjectHAnimInstance()
{
	bIsMoving = false;
	bIsRunning = false;
	/*Side = 0.0;
	BackTurn = 0.0f;
	bTurnFlag = false;*/
}


void UProjectHAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn) && OwnerCharacter)
	{
		CurrentSpeed = Pawn->GetVelocity().Size();
		bIsMoving = bMoving();
		TurnFunction();
	}
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


bool UProjectHAnimInstance::bMoving()
{
	CurrentAccel = MovementComponent->GetCurrentAcceleration().Size();
	return MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector && CurrentSpeed > 10.0f;
}

void UProjectHAnimInstance::TurnFunction()
{
	// 어디쪽으로 도는지 알수있는 방법. 캐릭터에서 각각 입력축과 이동하는 방향을 곱해서 여기로 건네줌.
	//FVector temp = UKismetMathLibrary::Normal(ForwardV + RightV);
	//BackTurn = FVector::DotProduct(temp, OwnerCharacter->GetActorForwardVector()); // BackTurn는 현재 바라보는 방향으로 가냐 안가냐를 나타내고
	//Side = FVector::DotProduct(temp, OwnerCharacter->GetActorRightVector()); // Side는 어느방향으로 도는가를 알아본다.

	// 입력축을 눌러서 현재 어디 방향으로 이동하고 있는지 알수있는 방법.
	//FRotator Rtemp = UKismetMathLibrary::NormalizedDeltaRotator(OwnerCharacter->GetControlRotation(), OwnerCharacter->GetVelocity().Rotation());

	FRotator Temp = UKismetMathLibrary::NormalizedDeltaRotator(OwnerCharacter->GetBaseAimRotation(), OwnerCharacter->GetActorRotation());
	Direction = Temp.Yaw * -1.f;
	Pitch = Temp.Pitch;
}

void UProjectHAnimInstance::TurnMontage(bool D, float TR)
{
	/*bTurnFlag = true;
	bDirection = D;
	TurnRate = 1.0f - TR;*/
}


void UProjectHAnimInstance::AnimNotify_Disable()
{
	OwnerCharacter->bPlay = false;
}

void UProjectHAnimInstance::AnimNotify_Enable()
{
	OwnerCharacter->bPlay = true;
}