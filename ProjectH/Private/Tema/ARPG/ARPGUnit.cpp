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

	// 해당 게임은 무기를 바꿀 필요가 없으므로 무기를 이렇게 지정했지만,
	// 소울류 게임에서처럼 무기를 변경 하는 게임인 경우는 인벤토리에서 직접 무기를 가져온다.
	// 때문에 LMB_AP, RMB_AP로 변수를 만들어 무기를 변경할때 휘두를 수있는 AP가 달리지게 설계할듯.

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
	// 소드 쉴드가 따로 있는 이유는 방패를 끼지않는 캐릭터도 존재 할 수 있기 때문.
	// (하지만 지금 이 게임은 존재하지 않는다.)
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
		//AttackCharget를 어떤식으로 게이지를 올릴 것인지.
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
	//일단 보류
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
	// 따로 이런 함수를 만든 이유는, 쉴드 뿐만아니라 특정 상황에서 못때리는 상태 일 수도 있으므로,
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

// TakeDamage AP 버전
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


//쉴드의 중요 한것은. 결국 쉴드에 오버랩되는 것이 중요한게아니라
//캐릭터가 닿았을때 Blocking 중인지가 중요하다. 굳이 쉴드 오버랩을 할 필요가없다.
//결국 오버랩은 무기가 공격할때 쓰는 쪽으로 사용한다.
void AARPGUnit::SetShieldCollision(bool bFlag)
{
	Shield->SetWeaponCollision(bFlag);
}


// 공격이 전부 다 끝났을때 호출.
void AARPGUnit::AttackEnd()
{
	bAttacking = false;
}

// 콤보 공격이나, 콜리전을 잠시 꺼야할 때 호출.
void AARPGUnit::WeaponOverlapEnd()
{
	_DEBUG("Owner Weapon AttackEnd");
	SetWeaponCollision(false);
	Weapon->AttackEnd();
}

// bHitting은 지금 그냥 변수만 있을뿐 딱히 활용 하지않고 있음 (23/01/05)
void AARPGUnit::HitEnd()
{
	bHitting = false;
}

// 방패 피격시 뭐 추가 기능이 있을수 있으니 방패 끄기 기능 함수를 따로 생성
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

// 적 락온
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









