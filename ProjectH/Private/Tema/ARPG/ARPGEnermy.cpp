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

	if (bMoving) // 이 변수는 Task에서 켜준다.
	{
		//_DEBUG("ParentTick");
		EnermyMoving(); 
		// 적이 플레이어가 시야에 들어왔을때 어떻게 움직일 것인지.
		// 다크소울이나 엘더스크롤 느낌으로 제작해야함. 원신이나 RPG류는 해당 방식이 어울리지 않음
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

	if (GetMesh()) // 가끔 C++ 클래스를 상속받은 블프가 디테일창이 사라지면서 null값을 넣기때문.(버그)
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

// 어느 방향의 Value인지 나타낸다.
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


// 플레이어 시선 고정
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


// 어디로 움직이는지 알수있는 함수.
float AARPGEnermy::GetDirection(FVector MoveDirection)
{
	FRotator Temp = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), MoveDirection.Rotation()); // or UKismetMathLibrary::MakeRotFromX(MoveDirection);

	return Temp.Yaw * -1.f;
}

void AARPGEnermy::SetCollisionRadius(bool bFlag)
{
	CurrentCollisionRadius = bFlag ? BattleCollisionRadius : NormalCollisionRadius;
}
