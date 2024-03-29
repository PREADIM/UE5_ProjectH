// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/AI/ARPG_EnermyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/ARPG/ARPGAIController.h"
#include "Tema/ARPG/ARPGAttackComponent.h"

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
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

	//★★ CreateDefaultSubobject를 하면 해당 클래스를 가진 액터들이 CDO를 공유하게 되므로 NewObject로 따로 할당.
	AttackComponent = NewObject<UARPGAttackComponent>(this, UARPGAttackComponent::StaticClass());
	AttackComponent->RegisterComponent();

	if (AttackComponent)
		AttackComponent->Init(BP_Attacks);
	
	UnitState.Init(this); // 스탯 초기화.
}

// Called every frame
void AARPGEnermy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bDeath)
		return;

	if (!bDontLockOn)
		LockOnPlayer(DeltaSeconds);

	if (!bDontMoving && !bHitting && bMoving) // 이 변수는 Task에서 켜준다.
	{
		SetState();
		GetMovingValue();
	}
}

void AARPGEnermy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


float AARPGEnermy::TakeDamageCalculator(AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser)
{
	return 0.f;
}


float AARPGEnermy::DamageFunction(class AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser)
{
	Super::DamageFunction(DamageWeapon, DamageEvent, EventInstigator, DamageCauser);

	if (PlayerUnit == nullptr)
	{
		AARPGUnit* Temp = Cast<AARPGUnit>(DamageCauser);
		if (Temp)
		{
			PlayerUnit = Temp;
		}
	}

	return 0.f;
}




void AARPGEnermy::Death()
{
	OwnerAIController->OnUnPossess();
	Super::Death();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	SetActorTickEnabled(false);
	//DeathCollsionEnabled();
	
}

void AARPGEnermy::ZeroAP()
{
	// 적은 AP가 곧 그로기 이기때문에 다르게 생성.
}





// 어느 방향의 Value인지 나타낸다.
FVector AARPGEnermy::GetMovingValue()
{
	FVector Direction;

	switch (MainMovingState)
	{
	case EMovingState::Stop :
		Direction = FVector::ZeroVector;
		break;
	case EMovingState::Forward:
		GetCharacterMovement()->MaxWalkSpeed = BattleSpeed;
		Direction = GetActorForwardVector();
		break;
	case EMovingState::Back:
		GetCharacterMovement()->MaxWalkSpeed = BackSpeed;
		Direction = GetActorForwardVector() * -1.f;
		break;
	case EMovingState::Right:
		GetCharacterMovement()->MaxWalkSpeed = BattleSpeed;
		Direction = GetActorRightVector();
		break;
	case EMovingState::Left:
		GetCharacterMovement()->MaxWalkSpeed = BattleSpeed;
		Direction = GetActorRightVector() * -1.f;
		break;
	}

	AddMovementInput(Direction, 1.0f);
	return Direction;
}


// 플레이어 시선 고정
void AARPGEnermy::LockOnPlayer(float DeltaSeconds)
{
	if (PlayerUnit)
	{
		FRotator LookRot = FindPlayerRotation();
		//SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));

		FRotator CurrentRot = GetActorRotation();
	
		FRotator Temp = FMath::RInterpTo(CurrentRot, LookRot, DeltaSeconds, 10.0f);
		SetActorRotation(FRotator(0.f, Temp.Yaw, 0.f));
	}
}

FRotator AARPGEnermy::FindPlayerRotation()
{
	if(PlayerUnit)
		return UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerUnit->GetActorLocation());

	return FRotator(0.f, 0.f, 0.f);
}

void AARPGEnermy::SetState()
{
	switch(State)
	{
	case EEnermyMoveMode::None:
		MainMovingState = EMovingState::Stop;
		break;
	case EEnermyMoveMode::ForwardMoving :
		MainMovingState = EMovingState::Forward;
		break;
	case EEnermyMoveMode::LeftMoving :
		MainMovingState = EMovingState::Left;
		break;
	case EEnermyMoveMode::RightMoving :
		MainMovingState = EMovingState::Right;
		break;
	case EEnermyMoveMode::BackMoving :
		MainMovingState = EMovingState::Back;
		break;
	}
}


void AARPGEnermy::SetBattleMode(bool bFlag)
{
	bBattleMode = bFlag;

	if (!bFlag)
	{
		UnitState.ResetPoise();
		PlayerUnit = nullptr;
	}
}

void AARPGEnermy::SetEnermyMoveMode(EEnermyMoveMode Mode)
{
	State = Mode;
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

float AARPGEnermy::GetBattleDistance()
{
	return GetAttackComponent()->MaxAttackDistance;
}
