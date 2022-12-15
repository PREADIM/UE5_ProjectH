// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/AI/ARPG_EnermyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/ARPG/ARPGUnit.h"

// Sets default values
AARPGEnermy::AARPGEnermy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	Tags.Add(FName("Enermy"));
}

// Called when the game starts or when spawned
void AARPGEnermy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AARPGEnermy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoving) // �� ������ Task���� ���ش�.
	{
		//_DEBUG("ParentTick");
		EnermyMoving(); 
		// ���� �÷��̾ �þ߿� �������� ��� ������ ������.
		// ��ũ�ҿ��̳� ������ũ�� �������� �����ؾ���. �����̳� RPG���� �ش� ����� ��︮�� ����
	}
}

// Called to bind functionality to input
void AARPGEnermy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AARPGEnermy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetMesh()) // ���� C++ Ŭ������ ��ӹ��� ������ ������â�� ������鼭 null���� �ֱ⶧��.(����)
	{
		EnermyAnimInstance = Cast<UARPG_EnermyAnimInstance>(GetMesh()->GetAnimInstance());
		if (EnermyAnimInstance != nullptr)
		{
			EnermyAnimInstance->CurrentCharacter = this;
		}
		else
		{
			_DEBUG("Not AI Has AnimInstance");
		}
	}
}

// ��� ������ Value���� ��Ÿ����.
FVector AARPGEnermy::GetMovingValue()
{
	FVector Direction;
	GetCharacterMovement()->MaxWalkSpeed = BattleSpeed;

	switch (EnermyMoveState)
	{
	case EMovingState::Forward:
		Direction = GetActorForwardVector();
		break;
	case EMovingState::Back:
		GetCharacterMovement()->MaxWalkSpeed -= 60.f;
		Direction = GetActorForwardVector() * -1.f;
		break;
	case EMovingState::Right:
		Direction = GetActorRightVector();
		break;
	case EMovingState::Left:
		Direction = GetActorRightVector() * -1.f;
		break;
	}

	AddMovementInput(Direction, 0.8f);

	return Direction;
}


// �÷��̾� �ü� ����
void AARPGEnermy::LockOnPlayer()
{
	if (PlayerUnit)
	{
		FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerUnit->GetActorLocation());
		SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));
	}
}

void AARPGEnermy::SetBlocking(bool bFlag)
{
	if (!EnermyAnimInstance)
		return;

	bBlocking = bFlag;
	EnermyAnimInstance->bBlocking = bFlag;
	

}

void AARPGEnermy::SetBattleMode(bool bFlag)
{
	if (!EnermyAnimInstance)
		return;

	bBattleMode = bFlag;
	EnermyAnimInstance->bBattleMode = bFlag;
}

void AARPGEnermy::SetEnermyMoveMode(EEnermyMoveMode Mode)
{
	State = Mode;
}

void AARPGEnermy::CastMoving()
{
	OnMoving.Broadcast();
}


// ���� �����̴��� �˼��ִ� �Լ�.
float AARPGEnermy::GetDirection(FVector MoveDirection)
{
	FRotator Temp = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), MoveDirection.Rotation()); // or UKismetMathLibrary::MakeRotFromX(MoveDirection);

	return Temp.Yaw * -1.f;
}

void AARPGEnermy::SetCollisionRadius(bool bFlag)
{
	CurrentCollisionRadius = bFlag ? BattleCollisionRadius : NormalCollisionRadius;
}
