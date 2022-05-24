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
	// ��������� ������ �˼��ִ� ���. ĳ���Ϳ��� ���� �Է���� �̵��ϴ� ������ ���ؼ� ����� �ǳ���.
	//FVector temp = UKismetMathLibrary::Normal(ForwardV + RightV);
	//BackTurn = FVector::DotProduct(temp, OwnerCharacter->GetActorForwardVector()); // BackTurn�� ���� �ٶ󺸴� �������� ���� �Ȱ��ĸ� ��Ÿ����
	//Side = FVector::DotProduct(temp, OwnerCharacter->GetActorRightVector()); // Side�� ����������� ���°��� �˾ƺ���.

	// �Է����� ������ ���� ��� �������� �̵��ϰ� �ִ��� �˼��ִ� ���.
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