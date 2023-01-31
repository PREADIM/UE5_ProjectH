// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/AI/ARPG_EnermyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"
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

	//�ڡ� CreateDefaultSubobject�� �ϸ� �ش� Ŭ������ ���� ���͵��� CDO�� �����ϰ� �ǹǷ� NewObject�� ���� �Ҵ�.
	AttackComponent = NewObject<UARPGAttackComponent>(this, UARPGAttackComponent::StaticClass());
	AttackComponent->RegisterComponent();

	if (AttackComponent)
		AttackComponent->Init(BP_Attacks);

	UnitState.Init(this); // ���� �ʱ�ȭ.
}

// Called every frame
void AARPGEnermy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bDeath && bMoving) // �� ������ Task���� ���ش�.
	{
		EnermyMoving();
	}
}

void AARPGEnermy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//float AARPGEnermy::TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	Super::TakeDamageCalculator(APDamage, DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//	if (PlayerUnit == nullptr)
//	{
//		AARPGUnit* Temp = Cast<AARPGUnit>(EventInstigator->GetPawn());
//		if (Temp)
//		{
//			PlayerUnit = Temp;
//		}
//	}
//
//
//	return DamageAmount;
//}


float AARPGEnermy::TakeDamageCalculator(AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamageCalculator(DamageWeapon, DamageEvent, EventInstigator, DamageCauser);

	if (PlayerUnit == nullptr)
	{
		AARPGUnit* Temp = Cast<AARPGUnit>(EventInstigator->GetPawn());
		if (Temp)
		{
			PlayerUnit = Temp;
		}
	}


	return 0.f;
}




void AARPGEnermy::Death()
{
	Super::Death();
	SetActorTickEnabled(false);
	DeathCollsionEnabled();
	OwnerAIController->OnUnPossess();
}

void AARPGEnermy::ZeroAP()
{
	// ���� AP�� �� �׷α� �̱⶧���� �ٸ��� ����.


}



// ��� ������ Value���� ��Ÿ����.
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
		GetCharacterMovement()->MaxWalkSpeed = BattleSpeed - 50.f;
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


// �÷��̾� �ü� ����
void AARPGEnermy::LockOnPlayer()
{
	if (PlayerUnit)
	{
		FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerUnit->GetActorLocation());
		SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));
	}
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
	if (bBattleMode == bFlag)
		return;

	bBattleMode = bFlag;
	ChangeBattleMode(bFlag);
}

void AARPGEnermy::SetEnermyMoveMode(EEnermyMoveMode Mode)
{
	State = Mode;
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

float AARPGEnermy::GetBattleDistance()
{
	return GetAttackComponent()->MaxAttackDistance;
}
