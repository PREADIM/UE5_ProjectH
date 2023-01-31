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
	FPSCamera->bUsePawnControlRotation = true;

	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMesh"));
	FPSMesh->SetupAttachment(FPSCamera);
	FPSMesh->SetOwnerNoSee(true);

	DeathCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DeathCamera"));
	DeathCamera->SetupAttachment(GetMesh());
	DeathCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "HeadSocket");

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
		TPSWeapon = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_Sword, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		FPSWeapon = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_Sword, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);

		if (TPSWeapon && FPSWeapon)
		{
			TPSWeapon->OwnerUnit = this;		
			FPSWeapon->OwnerUnit = this;

			TPSWeapon->SetOwnerNoSee(true);
			FPSWeapon->SetOwnerNoSee(false);

			TPSWeapon->FinishSpawning(FTransform());
			FPSWeapon->FinishSpawning(FTransform());

			LMB_AP = TPSWeapon->UseAP;
			UnitState.NormallyPoise += TPSWeapon->WeaponPoise;
			TPSWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("IdleSword"));
			FPSWeapon->AttachToComponent(FPSMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("IdleSword"));
		}
	}

	//RMB
	// 소드 쉴드가 따로 있는 이유는 방패를 끼지않는 캐릭터도 존재 할 수 있기 때문.
	// (하지만 지금 이 게임은 존재하지 않는다.)
	if (BP_Shield)
	{
		TPSShield = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_Shield, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		FPSShield = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_Shield, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);

		if (TPSShield && FPSShield)
		{
			TPSShield->OwnerUnit = this;
			FPSShield->OwnerUnit = this;

			TPSShield->SetOwnerNoSee(true);
			FPSShield->SetOwnerNoSee(false);

			TPSShield->FinishSpawning(FTransform());
			FPSShield->FinishSpawning(FTransform());

			RMB_AP = FPSShield->UseAP;
			TPSShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ShieldSocket"));
			FPSShield->AttachToComponent(FPSMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ShieldSocket"));
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

	OnUseAP.AddUFunction(this, FName("StartUseAPFunction"));
	OnUsingAP.AddUFunction(this, FName("UsingAPFunction"));
	OnEndAP.AddUFunction(this, FName("EndAPFunction"));

	UnitState.Init(this); // 스탯 초기화.
}

// Called every frame
void AARPGUnit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	if (bChargeAttacking)
	{
		//AttackCharge를 어떤식으로 게이지를 올릴 것인지.
		// 마우스를 누르고 있을때 해당 게이지가 오른다.
		if (FPSWeapon->IsChargeAttack())
		{
			OwnerController->SetChargeAttacking(FPSWeapon->ChargeAttack(DeltaSeconds));
		}
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
	TPSMeshAnimInstance = Cast<UARPG_TPSAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called to bind functionality to input
void AARPGUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &AARPGUnit::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AARPGUnit::LookRight);

	PlayerInputComponent->BindAxis("Forward", this, &AARPGUnit::Forward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AARPGUnit::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AARPGUnit::Jumping);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AARPGUnit::StopJump);

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
	if (bDeath)
		return;

	if (!bSpecialAttackPlaying || !bParringHit)
	{
		//AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), Value);
		//AddMovementInput(GetActorForwardVector(), Value);
		FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
		Direction.Z = 0.f;
		Direction.Normalize();
		AddMovementInput(Direction, Value);
	}
}

void AARPGUnit::MoveRight(float Value)
{
	if (bDeath)
		return;
	
	if (!bSpecialAttackPlaying || !bParringHit)
	{
		//AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), Value);
		//AddMovementInput(GetActorRightVector(), Value);
		FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
		Direction.Z = 0.f;
		Direction.Normalize();
		AddMovementInput(Direction, Value);
	}
}

void AARPGUnit::LookUp(float AxisValue)
{
	if (!bTargeting || !bParringHit)
		AddControllerPitchInput(AxisValue * MouseSensivity * GetWorld()->GetDeltaSeconds());
}

void AARPGUnit::LookRight(float AxisValue)
{
	if(!bTargeting || !bParringHit)
		AddControllerYawInput(AxisValue * MouseSensivity * GetWorld()->GetDeltaSeconds());
}


void AARPGUnit::SpecialAttack()
{
	bSpecialAttackMode = true;
	bSpecialAttackPlaying = true;
	UnitState.ATK = UnitState.NormallyATK * 2.0; // 일시적 펌핑
}

void AARPGUnit::ZeroAP()
{
	if (bSprint)
	{
		bSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	LMBReleased();
	Super::ZeroAP();
}

void AARPGUnit::DeathWeaponSimulate()
{
}

void AARPGUnit::SetDeathCamera()
{

}

void AARPGUnit::LMB()
{
	if (bDeath)
		return;

	if (!bBlockMode && CanUseAP())
	{
		bLMBPush = true;

		if (bCanParringAttack && !bSpecialAttackPlaying)
		{
			SpecialAttack();
			FPSMeshAnimInstance->ParringAttack();
			TPSMeshAnimInstance->ParringAttack();
		}

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
	if (bDeath)
		return;

	bLMBPush = false;
	bAttacking = false;
	AttackCharge = false;
	bAttackBackward = false;
	bAttackForward = false;
	bAttackLeft = false;
	bAttackRight = false;
	AttackCharge = 0.f;

	//EndAttack();
	//ResetMode();
}

void AARPGUnit::RMB()
{
	if (bDeath)
		return;

	if (bAttacking)
		return;

	bRMBPush = true;
	BlockingDEF = FPSShield->BlockingDEF;
	bCanParringAttack = false;
	bBlockMode = true;
}

void AARPGUnit::RMBReleased()
{
	if (bDeath)
		return;

	if (!bRMBPush)
		return;

	bRMBPush = false;
	bBlockMode = false;

	if (bUseAP)
		OnEndAP.Broadcast();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SetShieldCollision(false);
	BlockingEnd();
}

void AARPGUnit::Jumping()
{
	if (bDeath)
		return;

	Jump();

}

void AARPGUnit::StopJump()
{
	if (bDeath)
		return;

	StopJumping();

}

void AARPGUnit::Sprint()
{
	if (bDeath)
		return;

	if (bBlockMode)
		return;

	if (bAttacking)
		return;

	if(CanUseAP())
		bSprint = true;
}

void AARPGUnit::SprintReleased()
{
	if (bDeath)
		return;

	if (!bSprint)
		return;

	OnEndAP.Broadcast();

	bSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AARPGUnit::Sheathed()
{
	if (bDeath)
		return;

	if(bUseAP)
		OnEndAP.Broadcast();

	AttackEnd();

	bNormalMode = !bNormalMode;
	if (bNormalMode)
	{
		WalkSpeed = NormalSpeed;
		LMBReleased();
	}
	else
	{
		WalkSpeed = BattleSpeed;
	}

	FPSMeshAnimInstance->WeaponOnOff(bNormalMode);
	TPSMeshAnimInstance->WeaponOnOff(bNormalMode);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AARPGUnit::Parring()
{
	if (bBlocking && !bParringPlaying && !bHitting)
	{
		FPSMeshAnimInstance->Parring();
		TPSMeshAnimInstance->Parring();
	}
}


// ★★★
// AP 사용 함수들 각 역할 중요.
void AARPGUnit::StartUseAPFunction()
{
	_DEBUG("Start UseAP");
	bUseAP = true;
}

void AARPGUnit::UsingAPFunction()
{
	_DEBUG("Using AP");

	bUseAP = true; // 애초에 AP를 사용하려면 이게 당연히 true다. 스프린트에서 유용
	bUsingAP = true;
}

void AARPGUnit::EndAPFunction()
{
	_DEBUG("End UseAP");

	bUseAP = false;
	bUsingAP = false;
	bChargeAttacking = false;
}

void AARPGUnit::Death()
{
	Super::Death();
	ResetMode();
	bDeath = true;
	SetActorTickEnabled(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FPSMeshAnimInstance->Death();
	TPSMeshAnimInstance->Death();
}

void AARPGUnit::EndAttack()
{
	bLMBPush = false;
	bAttacking = false;
	AttackCharge = false;
	bAttackBackward = false;
	bAttackForward = false;
	bAttackLeft = false;
	bAttackRight = false;
	AttackCharge = 0.f;
	FPSWeapon->End();
	ChargeAttackEnd();
}

void AARPGUnit::ChargeAttackStart()
{
	bChargeAttacking = true;
	OwnerController->ChargeAttackInViewport(true);
}

void AARPGUnit::ChargeAttackEnd()
{
	bChargeAttacking = false;
	OwnerController->ChargeAttackInViewport(false);
}

bool AARPGUnit::Hit(bool bBlockingHit)
{
	if (!FPSMeshAnimInstance && !TPSMeshAnimInstance)
		return false;

	WeaponOverlapEnd();
	AttackEnd();

	bHitting = true;
	bParringPlaying = false;
	bParring = false;
	bParringHit = false;

	if (!bBlockingHit)
	{
		bool bHitMontagePlay = Super::Hit(bBlockingHit); // 강인도 검사
		// 적의 경우 AP는 그로기 여부이고, 강인도는 슈퍼아머냐 아니냐의 차이이다.

		if (bHitMontagePlay)
		{
			FPSMeshAnimInstance->Hit(EUnitMode::BattleMode);
			TPSMeshAnimInstance->Hit(EUnitMode::BattleMode);
			PlayCameraShake(BP_BattleMode_CS);
		}
	}
	else
	{
		FPSMeshAnimInstance->Hit(EUnitMode::BlockingMode);
		TPSMeshAnimInstance->Hit(EUnitMode::BlockingMode);
		PlayCameraShake(BP_BlockingMode_CS);
	}

	return true;
}


void AARPGUnit::ParringHit(AARPGUnitBase* InstigatorActor)
{
	bParringHit = true;
	bHitting = true;
	bBlocking = false;
	bBlockMode = false;

	//현재 적은 패링이 없다.
	//AnimInstance->ParringInstigatorUnit = InstigatorActor;
}


void AARPGUnit::PlayCameraShake(TSubclassOf<class UCameraShakeBase> CS)
{
	if (CS)
		OwnerController->ClientStartCameraShake(CS, 1.3f);
}

void AARPGUnit::ResetMode()
{
	bBlockMode = false;
	bBlocking = false;
	bParring = false;
	bParringHit = false;
	bParringPlaying = false;
	EndAttack();
}

//float AARPGUnit::TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	if (bDeath || bSpecialAttackMode)
//	{
//		return 0.0f;
//	}
//
//
//	float Damaged = DamageAmount;
//	float CurrentHP = UnitState.HP;
//	bool bBlockingHit = false;
//
//	if (bBlocking)
//	{
//		if (TargetDotProduct(DamageCauser->GetActorLocation(), 0.7)) // 45도 가량
//		{
//			Damaged = DamageAmount - (DamageAmount * BlockingDEF); // BlockingDEF는 0.0~1.0으로 되어있다.
//			TakeDamageAP(APDamage);		
//			bBlockingHit = true;
//		}
//	}
//	
//	if (CurrentHP <= Damaged)
//	{
//		Damaged = CurrentHP; // 남은 체력이 곧 라스트 데미지
//		CurrentHP = 0.f;
//		UnitState.SetTakeDamageHP(CurrentHP);
//		Death();
//	}
//	else
//	{
//		Hit(bBlockingHit);
//		if (Damaged > 0.f)
//		{
//			CurrentHP -= Damaged;
//			UnitState.SetTakeDamageHP(CurrentHP);
//		}
//	}
//
//	Super::TakeDamageCalculator(APDamage, Damaged, DamageEvent, EventInstigator, DamageCauser);
//
//	return Damaged;
//}


float AARPGUnit::TakeDamageCalculator(AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDeath || bSpecialAttackMode)
	{
		return 0.0f;
	}

	float TotalDamage = CalculDamage(DamageWeapon->WeaponDamage * DamageWeapon->Charge);

	float Damaged = TotalDamage;
	float CurrentHP = UnitState.HP;
	bool bBlockingHit = false;

	//무기 강인도를 이용해서 맞은 액터의 강인도를 깎는다.
	Super::TakeDamageCalculator(DamageWeapon, DamageEvent, EventInstigator, DamageCauser);

	if (bBlocking)
	{
		if (TargetDotProduct(DamageCauser->GetActorLocation(), 0.7)) // 45도 가량
		{
			float APDMG = CalculAPDamage(DamageWeapon->WeaponAP_DMG);
			Damaged = TotalDamage - (TotalDamage * BlockingDEF); // BlockingDEF는 0.0~1.0으로 되어있다.
			TakeDamageAP(APDMG);
			bBlockingHit = true;
		}
	}
	
	if (CurrentHP <= Damaged)
	{
		Damaged = CurrentHP; // 남은 체력이 곧 라스트 데미지
		CurrentHP = 0.f;
		UnitState.SetTakeDamageHP(CurrentHP);
		Death();
	}
	else
	{
		Hit(bBlockingHit);
		if (Damaged > 0.f)
		{
			CurrentHP -= Damaged;
			UnitState.SetTakeDamageHP(CurrentHP);
		}
	}

	// 데미지 출력하는 멀티캐스트.
	if (Damaged > 0.f)
	{
		OnDamage.Broadcast(Damaged);
	}

	return Damaged;
}



// TakeDamage AP 버전 블록킹했을때 AP깎는 함수.
void AARPGUnit::TakeDamageAP(float Damage)
{
	//여기서 만약 무슨 스킬로 인해 AP가 깎일 필요가없는경우 다른 함수를 실행하게한다.

	float CurrentAP = UnitState.AP;
	OnUseAP.Broadcast(); // 막아낸 공격이니 감소를 한다. 그뒤 잠시 멈춤

	if (CurrentAP <= Damage)
	{
		CurrentAP = 0.0f;
		UnitState.SetAP(CurrentAP);
		ShieldZeroAP();
		ZeroAP();
	}
	else
	{
		CurrentAP -= Damage;
		UnitState.SetAP(CurrentAP);
	}

}


void AARPGUnit::SetWeaponCollision(bool bFlag)
{
	FPSWeapon->SetWeaponCollision(bFlag);
}


//쉴드의 중요 한것은. 결국 쉴드에 오버랩되는 것이 중요한게아니라
//캐릭터가 닿았을때 Blocking 중인지가 중요하다. 굳이 쉴드 오버랩을 할 필요가없다.
//결국 오버랩은 무기가 공격할때 쓰는 쪽으로 사용한다.
void AARPGUnit::SetShieldCollision(bool bFlag)
{
	FPSWeapon->SetWeaponCollision(bFlag);
}


// 공격이 전부 다 끝났을때 호출.
void AARPGUnit::AttackEnd()
{
	//bAttacking = false;
	//몽타주 기반 공격도 아니고 섹션 점프도 없기때문에, 콤보 공격이 없어서,
	//그냥 LMBRelease 함수에서 false를 하기 때문에 굳이 하지 않아도 된다.
	EndAttack();

}

// 콤보 공격이나, 콜리전을 잠시 꺼야할 때 호출.
void AARPGUnit::WeaponOverlapEnd()
{
	_DEBUG("Owner Weapon AttackEnd");
	SetWeaponCollision(false);
	FPSWeapon->AttackEnd();
}

// bHitting은 맞고있을때 다른 애니메이션을 실행하지 않도록 하기위함.
void AARPGUnit::HitEnd()
{
	bHitting = false;
	bParringHit = false;

	//맞고 나서도 RMB가 눌려져있으면 블럭킹이 알아서 되게 한다.
	if (bRMBPush)
	{
		bBlockMode = true;
	}
}

// 방패 피격시 뭐 추가 기능이 있을수 있으니 방패 끄기 기능 함수를 따로 생성
void AARPGUnit::BlockingEnd()
{
	bBlocking = false;
	HitEnd();
}

void AARPGUnit::ShieldZeroAP()
{
	RMBReleased();
	PlayCameraShake(BP_BlockingZeroAPMode_CS);
	FPSMeshAnimInstance->ShieldZeroAP();
	TPSMeshAnimInstance->ShieldZeroAP();
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









