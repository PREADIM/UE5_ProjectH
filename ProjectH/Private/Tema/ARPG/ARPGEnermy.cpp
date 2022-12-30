// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/AI/ARPG_EnermyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Tema/ARPG/ARPGAttackComponent.h"

// Sets default values
AARPGEnermy::AARPGEnermy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackComponent = CreateDefaultSubobject<class UARPGAttackComponent>(TEXT("AttackComponent"));

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	Tags.Add(FName("Enermy"));
}

// Called when the game starts or when spawned
void AARPGEnermy::BeginPlay()
{
	Super::BeginPlay();

	if(AttackComponent)
		AttackComponent->Init(BP_Attacks);

	SetDynamicDelegateBind();
}

// Called every frame
void AARPGEnermy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoving) // �� ������ Task���� ���ش�.
	{
		EnermyMoving(); // �� ���ʹ̸��� �ٸ��Ƿ� �ϴ� �ش� �Լ��� ������ �������� ����.

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

float AARPGEnermy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	return DamageAmount;
}



// ��� ������ Value���� ��Ÿ����.
FVector AARPGEnermy::GetMovingValue()
{
	FVector Direction;

	switch (EnermyMoveState)
	{
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