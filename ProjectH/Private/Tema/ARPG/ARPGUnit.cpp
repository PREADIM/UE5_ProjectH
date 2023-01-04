// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGUnit.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Tema/ARPG/ARPGPlayerController.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetmathLibrary.h"
#include "Tema/ARPG/ARPG_UnitAnimInstance.h"

// Sets default values
AARPGUnit::AARPGUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(RootComponent);

	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMesh"));
	FPSMesh->SetupAttachment(FPSCamera);


	NormalSpeed = 250.f;
	BattleSpeed = 150.f;
	BlockSpeed = 130.f;
	WalkSpeed = NormalSpeed;
	LockOnRadius = 750.f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bNormalMode = true;

	Tags.Add(FName("Player"));
}

// Called when the game starts or when spawned
void AARPGUnit::BeginPlay()
{
	Super::BeginPlay();

	// �ش� ������ ���⸦ �ٲ� �ʿ䰡 �����Ƿ� ���⸦ �̷��� ����������,
	// �ҿ�� ���ӿ���ó�� ���⸦ ���� �ϴ� ������ ���� �κ��丮���� ���� ���⸦ �����´�.
	// ������ LMB_AP, RMB_AP�� ������ ����� ���⸦ �����Ҷ� �ֵθ� ���ִ� AP�� �޸����� �����ҵ�.

	//LMB
	if (BP_Sword)
	{
		Weapon = GetWorld()->SpawnActor<AARPGWeapon>(BP_Sword);		
		if (Weapon)
		{
			Weapon->OwnerUnit = this;		
			Weapon->SetOwner(this);
			LMB_AP = Weapon->UseAP;
			Weapon->AttachToComponent(FPSMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("IdleSword"));
		}
	}

	//RMB
	// �ҵ� ���尡 ���� �ִ� ������ ���и� �����ʴ� ĳ���͵� ���� �� �� �ֱ� ����.
	// (������ ���� �� ������ �������� �ʴ´�.)
	if (BP_Shield)
	{
		Shield = GetWorld()->SpawnActor<AARPGWeapon>(BP_Shield);

		if (Shield)
		{
			Shield->OwnerUnit = this;
			Shield->SetOwner(this);
			RMB_AP = Weapon->UseAP;
			Shield->AttachToComponent(FPSMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ShieldSocket"));
		}
	}

	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		MouseSensivity = GI->MS;
	}	
	OwnerController = Cast<AARPGPlayerController>(GetController());


	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel12));
	IgnoreActor.Add(this);
}

// Called every frame
void AARPGUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bAttacking)
	{
		//AttackCharget�� ������� �������� �ø� ������.
	}

	if (bTargeting)
	{
		if (TargetingPawn)
		{
			LockOnSetPosition(TargetingPawn->GetActorLocation());
			FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetingPawn->GetActorLocation());
			OwnerController->SetControlRotation(Rot);

			if (GetDistanceTo(TargetingPawn) > LockOnRadius)
			{
				bTargeting = false;
				TargetingPawn = nullptr;
			}
		}
	}
}

void AARPGUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FPSMeshAnimInstance = Cast<UARPG_UnitAnimInstance>(FPSMesh->GetAnimInstance());
}

// Called to bind functionality to input
void AARPGUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &AARPGUnit::Forward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AARPGUnit::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AARPGUnit::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AARPGUnit::LookRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AARPGUnit::LMB);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AARPGUnit::LMBReleased);

	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &AARPGUnit::RMB);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &AARPGUnit::RMBReleased);

	PlayerInputComponent->BindAction("Shift", IE_Pressed, this, &AARPGUnit::Sprint);
	PlayerInputComponent->BindAction("Shift", IE_Released, this, &AARPGUnit::SprintReleased);

	PlayerInputComponent->BindAction("R", IE_Pressed, this, &AARPGUnit::Sheathed);
	PlayerInputComponent->BindAction("F", IE_Pressed, this, &AARPGUnit::Parring);

	PlayerInputComponent->BindAction("LCntl", IE_Pressed, this, &AARPGUnit::LockOn);
	//�ϴ� ����
}

void AARPGUnit::Forward(float Value)
{
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), Value);
	//AddMovementInput(GetActorForwardVector(), Value);
	FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
	Direction.Z = 0.f;
	Direction.Normalize();
	AddMovementInput(Direction, Value);
}

void AARPGUnit::MoveRight(float Value)
{
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), Value);
	//AddMovementInput(GetActorRightVector(), Value);
	FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
	Direction.Z = 0.f;
	Direction.Normalize();
	AddMovementInput(Direction, Value);
}

void AARPGUnit::LookUp(float AxisValue)
{
	if (!bTargeting)
		AddControllerPitchInput(AxisValue * MouseSensivity * GetWorld()->GetDeltaSeconds());
}

void AARPGUnit::LookRight(float AxisValue)
{
	if(!bTargeting)
		AddControllerYawInput(AxisValue * MouseSensivity * GetWorld()->GetDeltaSeconds());
}

void AARPGUnit::LMB()
{
	if (!bBlockMode || UnitState.AP >= LMB_AP)
	{
		bAttacking = true;
		if (InputComponent->GetAxisValue(TEXT("MoveRight")) == 0.0f)
		{
			if (InputComponent->GetAxisValue(TEXT("Forward")) < 0.0f)
			{
				bAttackBackward = true;
			}
			else
			{
				bAttackForward = true;
			}
		}
		else if (InputComponent->GetAxisValue(TEXT("MoveRight")) < 0.0f)
		{
			bAttackLeft = true;
		}
		else
		{
			bAttackRight = true;
		}
	}
}

void AARPGUnit::LMBReleased()
{
	bAttacking = false;
	bAttackBackward = false;
	bAttackForward = false;
	bAttackLeft = false;
	bAttackRight = false;
	AttackCharge = 0.f;
}

void AARPGUnit::RMB()
{
	if (UnitState.AP >= RMB_AP)
	{
		bBlockMode = true;
		GetCharacterMovement()->MaxWalkSpeed = BlockSpeed;
	}
}

void AARPGUnit::RMBReleased()
{
	bBlockMode = false;
	bBlocking = false;
	bParring = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SetShieldCollision(false);
	BlockingEnd();
}

void AARPGUnit::Sprint()
{
	if (bBlockMode)
		return;

	bSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed + SprintSpeed;
}

void AARPGUnit::SprintReleased()
{
	bSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AARPGUnit::Sheathed()
{
	bNormalMode = !bNormalMode;
	if (bNormalMode)
	{
		WalkSpeed = NormalSpeed;
		bParring = false;
		LMBReleased();
	}
	else
	{
		WalkSpeed = BattleSpeed;
	}

	FPSMeshAnimInstance->WeaponOnOff(bNormalMode);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AARPGUnit::Parring()
{
	if(bBlocking && !bParring)
		bParring = true;

}

void AARPGUnit::Death()
{
	bDeath = true;
	FPSMeshAnimInstance->Death();
}

void AARPGUnit::Hit()
{
	if (!FPSMeshAnimInstance)
		return;

	bHitting = true;
	if (bBlocking)
	{
		FPSMeshAnimInstance->Hit(EUnitMode::BlockingMode);
		PlayCameraShake(BP_BlockingMode_CS);
	}
	else
	{
		FPSMeshAnimInstance->Hit(EUnitMode::BattleMode);
		PlayCameraShake(BP_BattleMode_CS);
	}
}

bool AARPGUnit::CanThisDamage()
{
	// ���� �̷� �Լ��� ���� ������, ���� �Ӹ��ƴ϶� Ư�� ��Ȳ���� �������� ���� �� ���� �����Ƿ�,
	if (bBlocking)
		return false;

	if (bDeath)
		return false;

	return true;
}

void AARPGUnit::PlayCameraShake(TSubclassOf<class UCameraShakeBase> CS)
{
	if (CS)
		OwnerController->ClientPlayCameraShake(CS, 1.3f);
}

float AARPGUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	WeaponOverlapEnd();
	AttackEnd();

	float CurrentHP = UnitState.HP;
	if (CurrentHP <= DamageAmount)
	{
		CurrentHP = 0.0f;
		UnitState.SetTakeDamageHP(CurrentHP);
		Death();
		return DamageAmount;
	}

	CurrentHP -= DamageAmount;
	Hit();
	UnitState.SetTakeDamageHP(CurrentHP);

	return DamageAmount;
}

// TakeDamage AP ����
void AARPGUnit::TakeDamageAP(float Damage)
{
	WeaponOverlapEnd();
	AttackEnd();

	float CurrentAP = UnitState.AP;

	if (CurrentAP <= Damage)
	{
		CurrentAP = 0.0f;
		UnitState.SetTakeDamageAP(CurrentAP);
		ZeroAP();
		return;
	}

	CurrentAP -= Damage;
	Hit();
	UnitState.SetTakeDamageAP(CurrentAP);	
}


void AARPGUnit::SetWeaponCollision(bool bFlag)
{
	Weapon->SetWeaponCollision(bFlag);
}


//������ �߿� �Ѱ���. �ᱹ ���忡 �������Ǵ� ���� �߿��ѰԾƴ϶�
//ĳ���Ͱ� ������� Blocking �������� �߿��ϴ�. ���� ���� �������� �� �ʿ䰡����.
//�ᱹ �������� ���Ⱑ �����Ҷ� ���� ������ ����Ѵ�.
void AARPGUnit::SetShieldCollision(bool bFlag)
{
	Shield->SetWeaponCollision(bFlag);
}


// ������ ���� �� �������� ȣ��.
void AARPGUnit::AttackEnd()
{
	bAttacking = false;
}

// �޺� �����̳�, �ݸ����� ��� ������ �� ȣ��.
void AARPGUnit::WeaponOverlapEnd()
{
	_DEBUG("Owner Weapon AttackEnd");
	SetWeaponCollision(false);
	Weapon->AttackEnd();
}

// bHitting�� ���� �׳� ������ ������ ���� Ȱ�� �����ʰ� ���� (23/01/05)
void AARPGUnit::HitEnd()
{
	bHitting = false;
}

// ���� �ǰݽ� �� �߰� ����� ������ ������ ���� ���� ��� �Լ��� ���� ����
void AARPGUnit::BlockingEnd()
{
	HitEnd();
}

void AARPGUnit::ZeroAP()
{
	RMBReleased();
	PlayCameraShake(BP_BlockingZeroAPMode_CS);
	FPSMeshAnimInstance->ZeroAP();

}

// �� ����
void AARPGUnit::LockOn()
{
	if (bTargeting)
	{
		bTargeting = false;
		LockOnAddViewport(false);
		return;
	}


	TArray<AActor*> OutActors;
	APawn* TargetPawn = nullptr;

	bool bResult = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), LockOnRadius, ObjectType, nullptr, IgnoreActor, OutActors);
	DrawDebugSphere(GetWorld(), GetActorLocation(), LockOnRadius, 20, bResult ? FColor::Green : FColor::Red, false, 3.0f);

	if (bResult)
	{
		float Distance = LockOnRadius;
		for (auto OutActor : OutActors)
		{
			float temp = GetDistanceTo(OutActor);
			Distance > temp ? temp : Distance;
			TargetPawn = Cast<APawn>(OutActor);
			if (TargetPawn)
			{
				TargetingPawn = TargetPawn;
				bTargeting = true;
				LockOnAddViewport(true);
				_DEBUG("Lock True");
			}
		}

		return;
	}
}

void AARPGUnit::LockOnSetPosition(FVector TargetPos)
{
	if (OwnerController)
	{
		FVector2D Pos;
		OwnerController->ProjectWorldLocationToScreen(TargetPos, Pos);
		OwnerController->SetLockPosition(Pos);
	}
}

void AARPGUnit::LockOnAddViewport(bool bFlag)
{
	OwnerController->LockOnAddViewport(bFlag);
}









