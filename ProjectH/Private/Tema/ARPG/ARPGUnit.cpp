// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGUnit.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Tema/ARPG/ARPGPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetmathLibrary.h"
#include "Tema/ARPG/ARPG_UnitAnimInstance.h"
#include "Tema/ARPG/ARPGGameMode.h"
#include <LevelSequencePlayer.h>
#include <LevelSequenceActor.h>

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

	SpecialAttackCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SpecialAttackCamera"));
	SpecialAttackCamera->SetupAttachment(RootComponent);
	SpecialAttackCamera->SetActive(false);

	DeathCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DeathCamera"));
	DeathCamera->SetupAttachment(GetMesh());
	DeathCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "HeadSocket");
	DeathCamera->SetActive(false);

	NormalSpeed = 250.f;
	BattleSpeed = 150.f;
	BlockSpeed = 130.f;
	WalkSpeed = NormalSpeed;
	//LockOnRadius = 750.f;

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
		//TPSWeapon = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_Sword, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		FPSWeapon = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_Sword, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);

		//if (TPSWeapon && FPSWeapon)
		if (FPSWeapon)
		{
			//TPSWeapon->OwnerUnit = this;				
			//TPSWeapon->SetOwner(this);

			FPSWeapon->OwnerUnit = this;
			FPSWeapon->SetOwner(this);

			//TPSWeapon->SetOwnerNoSee(true);
			// ������ ������������, �ִϸ��̼��� ��� �׳� ������.

			FPSWeapon->SetOwnerNoSee(false);
			LMB_AP = FPSWeapon->UseAP;
			UnitState.NormallyPoise += FPSWeapon->WeaponPoise;
	
			FPSWeapon->AttachToComponent(FPSMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("IdleSword"));
			FPSWeapon->FinishSpawning(FTransform());	

			//TPSWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("IdleSword"));
			//TPSWeapon->FinishSpawning(FTransform());
		}
	}

	//RMB
	// �ҵ� ���尡 ���� �ִ� ������ ���и� �����ʴ� ĳ���͵� ���� �� �� �ֱ� ����.
	// (������ ���� �� ������ �������� �ʴ´�.)
	if (BP_Shield)
	{
		//TPSShield = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_Shield, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		FPSShield = GetWorld()->SpawnActorDeferred<AARPGWeapon>(BP_Shield, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);

		//if (TPSShield && FPSShield)
		if (FPSShield)
		{
			//TPSShield->OwnerUnit = this;		
			//TPSShield->SetOwner(this);

			FPSShield->OwnerUnit = this;
			FPSShield->SetOwner(this);

			//TPSShield->SetOwnerNoSee(true);
			//TPSShield->FinishSpawning(FTransform());
			//TPSShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ShieldSocket"));

			FPSShield->SetOwnerNoSee(false);
			FPSShield->FinishSpawning(FTransform());				
			FPSShield->AttachToComponent(FPSMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ShieldSocket"));

			RMB_AP = FPSShield->UseAP;
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

	OnAttackAP.AddUFunction(this, FName("BindAttackAPDelegate"));
	OnWeaponDraw.BindUFunction(this, FName("BindWeaponDrawDelegate"));
	OnWeaponStow.BindUFunction(this, FName("BindWeaponStowDelegate"));


	UnitState.Init(this); // ���� �ʱ�ȭ.
}

// Called every frame
void AARPGUnit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	if (bChargeAttacking)
	{
		//AttackCharge�� ������� �������� �ø� ������.
		// ���콺�� ������ ������ �ش� �������� ������.
		if (FPSWeapon->IsChargeAttack())
		{
			OwnerController->SetChargeAttacking(FPSWeapon->ChargeAttack(DeltaSeconds));
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

	PlayerInputComponent->BindAction("LCntl", IE_Pressed, this, &AARPGUnit::Death);
	//�ϴ� ����
}

void AARPGUnit::Forward(float Value)
{
	if (bDeath)
		return;

	if (!bSpecialAttackMode && !bParringHit)
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
	
	if (!bSpecialAttackMode && !bParringHit)
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
	if (!bParringHit && !bSpecialAttackMode)
		AddControllerPitchInput(AxisValue * MouseSensivity * GetWorld()->GetDeltaSeconds());
}

void AARPGUnit::LookRight(float AxisValue)
{
	if (!bParringHit && !bSpecialAttackMode)
		AddControllerYawInput(AxisValue * MouseSensivity * GetWorld()->GetDeltaSeconds());
}


void AARPGUnit::SpecialAttack()
{
	FRotator R = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CanSATargetUnit->GetActorLocation());
	R = FRotator(0.f, R.Yaw + 180.f, 0.f); // ��Ÿ�ְ� �ڸ� �����ֱ⶧��.
	FVector L = CanSATargetUnit->GetActorLocation() + (CanSATargetUnit->GetActorForwardVector() * 20.f);

	OwnerController->SetControlRotation(R);
	SetActorLocation(L);
	
	bSpecialAttackMode = true;
	bSpecialAttackPlaying = true;
	UnitState.ATK = UnitState.NormallyATK * 4.0; // �Ͻ��� ����
	
	SetFPSMeshOwnerNoSee(true);
	SetTPSMeshOwnerNoSee(false);

	DeathCamera->SetActive(false);
	FPSCamera->SetActive(false);
	SpecialAttackCamera->SetActive(true);
}


void AARPGUnit::ParringSwingSFX()
{
	// �и� ���� ���� ���
	if (FPSShield)
	{
		FPSShield->PlayWeaponSound(EWeaponSFX::SwingSFX);
	}
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
	// ������ ����ִ� ���⸦ ���Ͽ��� ��� ������ �ù�
	FDetachmentTransformRules Rule(EDetachmentRule::KeepWorld, false);

	/*if (TPSWeapon)
	{
		TPSWeapon->DetachFromActor(Rule);
		TPSWeapon->SetPhysics();
	}

	if (TPSShield)
	{
		TPSShield->DetachFromActor(Rule);
		TPSShield->SetPhysics();
	}*/
}

void AARPGUnit::SetDeathCamera()
{
	// ���� ó�� ī�޶�
}

void AARPGUnit::LMB()
{
	if (bDeath)
		return;

	if (bNormalMode)
		return;

	if (!bBlockMode && CanUseAP())
	{
		bLMBPush = true;

		if (bCanParringAttack && !bSpecialAttackPlaying)
		{
			if (CanSATargetUnit)
			{
				FDamageEvent DamageEvent;
				SpecialAttack();
				FPSMeshAnimInstance->ParringAttack();
				TPSMeshAnimInstance->ParringAttack(); // ��Ʈ����� ����Ʈ �޽ÿ��� ����ǹǷ�. ���ý���
				CanSATargetUnit->EndAttack();
				CanSATargetUnit->SpecialAttackHitMontage();
				CanSATargetUnit->DamageFunction(FPSWeapon, DamageEvent, OwnerController, this); 
				// ���� TakeDamageCalcul �Լ��� �������� �������� �̷��� �ƹ��� ���Ǿ��� �ٷ� �������� ������ �ϴ� ����
				// �ٷ� ������ ó�� �Լ��� ����
			}

		}
		else if(!FPSMeshAnimInstance->bMontagePlaying && !bAttacking) // ��Ÿ�� ���� ���� �ƴ� ���
		{
			bAttacking = true;
			
			OnChargeAttackInit.Broadcast();
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
}

void AARPGUnit::LMBReleased()
{
	if (bDeath)
		return;

	bLMBPush = false;
	bAttacking = false;
	bAttackBackward = false;
	bAttackForward = false;
	bAttackLeft = false;
	bAttackRight = false;
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

	if (!FPSMeshAnimInstance->IsMoving)
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

	FPSMeshAnimInstance->WeaponOnOff(!bNormalMode);
	//TPSMeshAnimInstance->WeaponOnOff(!bNormalMode);
}

void AARPGUnit::Parring()
{
	if (bBlocking && !bParringPlaying && !bHitting)
	{
		FPSMeshAnimInstance->Parring();
		//TPSMeshAnimInstance->Parring();
	}
}


// �ڡڡ�
// AP ��� �Լ��� �� ���� �߿�.
void AARPGUnit::StartUseAPFunction()
{
	//_DEBUG("Start UseAP");
	bUseAP = true;
}

void AARPGUnit::UsingAPFunction()
{
	//_DEBUG("Using AP");

	bUseAP = true; // ���ʿ� AP�� ����Ϸ��� �̰� �翬�� true��. ������Ʈ���� ����
	bUsingAP = true;
}

void AARPGUnit::EndAPFunction()
{
	//_DEBUG("End UseAP");

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
	
	if (DeathCamera)
	{
		SpecialAttackCamera->SetActive(false);
		FPSCamera->SetActive(false);
		DeathCamera->SetActive(true);
	}

	// ���⼭ �״� �ó׸�ƽ ����.

	float SequenceEndTime = 5.f;
	ALevelSequenceActor* LQActor;
	if (DeathSequence)
	{
		ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), DeathSequence, FMovieSceneSequencePlaybackSettings(), LQActor);
		if (Player)
		{
			Player->Play();
			SequenceEndTime = Player->GetEndTime().AsSeconds();
		}
	}

	FTimerHandle DeathHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathHandle, this, &AARPGUnit::CallRestart, SequenceEndTime, false);
}

void AARPGUnit::CallRestart()
{
	if(!GM)
		GM = Cast<AARPGGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
		GM->Restart();
}


void AARPGUnit::EndAttack()
{
	bLMBPush = false;
	bAttacking = false;
	bAttackBackward = false;
	bAttackForward = false;
	bAttackLeft = false;
	bAttackRight = false;
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

void AARPGUnit::SetBossHPWidget(bool bFlag, AARPGEnermy* Boss)
{
	OwnerController->SetBossHPWidget(bFlag, Boss);
}

bool AARPGUnit::IsBossHPWidget()
{
	if (OwnerController->WidgetBossHP)
	{
		return OwnerController->IsBossHPWidget();
	}
	return false;
}

void AARPGUnit::SpecialAttackEnd()
{
	bSpecialAttackMode = false;
	bSpecialAttackPlaying = false;
	bCanParringAttack = false;
	OnEndAP.Broadcast();
	UnitState.ATK = UnitState.NormallyATK;

	FPSCamera->SetActive(true);
	SpecialAttackCamera->SetActive(false);

	SetFPSMeshOwnerNoSee(false);
	SetTPSMeshOwnerNoSee(true);
}

void AARPGUnit::SetFPSMeshOwnerNoSee(bool bFlag)
{
	FPSMesh->SetOwnerNoSee(bFlag);
	if(FPSWeapon)
		FPSWeapon->SetOwnerNoSee(bFlag);
	if(FPSShield)
		FPSShield->SetOwnerNoSee(bFlag);
}

void AARPGUnit::SetTPSMeshOwnerNoSee(bool bFlag)
{
	GetMesh()->SetOwnerNoSee(bFlag);
}

void AARPGUnit::BindAttackAPDelegate()
{
	bAttackAndHitAP = true;
}

void AARPGUnit::BindWeaponDrawDelegate()
{
	WalkSpeed = BattleSpeed;
	LMBReleased();
	bNormalMode = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AARPGUnit::BindWeaponStowDelegate()
{
	WalkSpeed = NormalSpeed;
	bNormalMode = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}




bool AARPGUnit::Hit(bool bBlockingHit)
{
	if (!FPSMeshAnimInstance && !TPSMeshAnimInstance)
		return false;


	if (!bBlockingHit)
	{
		bool bHitMontagePlay = Super::Hit(bBlockingHit); // ���ε� �˻�
		// ���� ��� AP�� �׷α� �����̰�, ���ε��� ���۾Ƹӳ� �ƴϳ��� �����̴�.

		if (bHitMontagePlay)
		{
			bHitting = true;
			bParringPlaying = false;
			bParring = false;
			bParringHit = false;

			FPSMeshAnimInstance->Hit(EUnitMode::BattleMode);
			//TPSMeshAnimInstance->Hit(EUnitMode::BattleMode);

			if (bAttacking)
			{
				WeaponOverlapEnd();
				//AttackEnd();
			}

			PlayCameraShake(BP_BattleMode_CS);
		}
		else
		{
			PlayCameraShake(BP_PoiseHitMode_CS);

			// bHitting�� ��Ʈ��ǽ� �ٸ� �ִϸ��̼��� �����ϴ� ���� �����ϴ� ���ε�,
			// ���ε��� ����Ͽ� ��Ʈ����� �߻�����������쿡�� �׳� false ó���ؾ��Ѵ�.
			// �׷��� �ٸ� �ִϸ��̼� ���డ�� ex) �и������ Hitting�߿� �����������.
			//bHitting = false; 
		}
	}
	else
	{
		bHitting = true;
		bParringPlaying = false;
		bParring = false;
		bParringHit = false;

		FPSMeshAnimInstance->Hit(EUnitMode::BlockingMode);
		//TPSMeshAnimInstance->Hit(EUnitMode::BlockingMode);
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

	//���� ���� �и��� ����.
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

float AARPGUnit::TakeDamageCalculator(AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser)
{
	if (bDeath || bSpecialAttackMode)
	{
		_DEBUG("SpecialMode");
		return 0.0f;
	}

	float Damaged = DamageCauser->CalculDamage(DamageWeapon->WeaponDamage * DamageWeapon->Charge);
	float CurrentHP = UnitState.HP;
	bool bBlockingHit = false;

	//���� ���ε��� �̿��ؼ� ���� ������ ���ε��� ��´�.
	Super::TakeDamageCalculator(DamageWeapon, DamageEvent, EventInstigator, DamageCauser);

	if (bBlocking)
	{
		if (TargetDotProduct(DamageCauser->GetActorLocation(), 0.7)) // 45�� ����
		{
			float APDMG = DamageCauser->CalculAPDamage(DamageWeapon->WeaponAP_DMG);
			Damaged = Damaged - (Damaged * BlockingDEF); // BlockingDEF�� 0.0~1.0���� �Ǿ��ִ�.
			TakeDamageAP(APDMG);
			OnAttackAP.Broadcast();
			bBlockingHit = true;
		}
	}
	

	if (CurrentHP <= Damaged)
	{
		Damaged = CurrentHP; // ���� ü���� �� ��Ʈ ������
		CurrentHP = 0.f;
		UnitState.SetTakeDamageHP(CurrentHP);
		Death();
		OnDamage.Broadcast(Damaged); // ������ ����ϴ� ��Ƽĳ��Ʈ.
	}
	else 
	{
		Hit(bBlockingHit);
		if (Damaged > 0.f)
		{
			CurrentHP -= Damaged;
			UnitState.SetTakeDamageHP(CurrentHP);
			OnDamage.Broadcast(Damaged); // ������ ����ϴ� ��Ƽĳ��Ʈ.
		}
	}

	return Damaged;
}



// TakeDamage AP ���� ���ŷ������ AP��� �Լ�.
void AARPGUnit::TakeDamageAP(float Damage)
{
	//���⼭ ���� ���� ��ų�� ���� AP�� ���� �ʿ䰡���°�� �ٸ� �Լ��� �����ϰ��Ѵ�.

	float CurrentAP = UnitState.AP;
	OnUseAP.Broadcast(); // ���Ƴ� �����̴� ���Ҹ� �Ѵ�. �׵� ��� ����

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

	// �ᱹ ���⸶�� ������ SFX�� �ٸ��⶧���� ���⿡�� �����ϰ��Ѵ�.
	if(bFlag)
		FPSWeapon->PlayWeaponSound(EWeaponSFX::SwingSFX);
	
}


//������ �߿� �Ѱ���. �ᱹ ���忡 �������Ǵ� ���� �߿��ѰԾƴ϶�
//ĳ���Ͱ� ������� Blocking �������� �߿��ϴ�. ���� ���� �������� �� �ʿ䰡����.
//�ᱹ �������� ���Ⱑ �����Ҷ� ���� ������ ����Ѵ�.
void AARPGUnit::SetShieldCollision(bool bFlag)
{
	FPSWeapon->SetWeaponCollision(bFlag);
}


// ������ ���� �� �������� ȣ��.
void AARPGUnit::AttackEnd()
{
	//bAttacking = false;
	//��Ÿ�� ��� ���ݵ� �ƴϰ� ���� ������ ���⶧����, �޺� ������ ���,
	//�׳� LMBRelease �Լ����� false�� �ϱ� ������ ���� ���� �ʾƵ� �ȴ�.
	EndAttack();
}

// �޺� �����̳�, �ݸ����� ��� ������ �� ȣ��.
void AARPGUnit::WeaponOverlapEnd()
{
	SetWeaponCollision(false);
	FPSWeapon->AttackEnd();
}

// bHitting�� �°������� �ٸ� �ִϸ��̼��� �������� �ʵ��� �ϱ�����.
void AARPGUnit::HitEnd()
{
	bHitting = false;
	bParringHit = false;
	AttackEnd();

	//�°� ������ RMB�� ������������ ��ŷ�� �˾Ƽ� �ǰ� �Ѵ�.
	if (bRMBPush)
	{
		bBlockMode = true;
	}
}

// ���� �ǰݽ� �� �߰� ����� ������ ������ ���� ���� ��� �Լ��� ���� ����
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
	//TPSMeshAnimInstance->ShieldZeroAP();
}


// �� ����
//void AARPGUnit::LockOn()
//{
//	if (bTargeting)
//	{
//		bTargeting = false;
//		LockOnAddViewport(false);
//		return;
//	}
//
//
//	TArray<AActor*> OutActors;
//	APawn* TargetPawn = nullptr;
//
//	bool bResult = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), LockOnRadius, ObjectType, nullptr, IgnoreActor, OutActors);
//	DrawDebugSphere(GetWorld(), GetActorLocation(), LockOnRadius, 20, bResult ? FColor::Green : FColor::Red, false, 3.0f);
//
//	if (bResult)
//	{
//		float Distance = LockOnRadius;
//		for (auto OutActor : OutActors)
//		{
//			float temp = GetDistanceTo(OutActor);
//			Distance > temp ? temp : Distance;
//			TargetPawn = Cast<APawn>(OutActor);
//			if (TargetPawn)
//			{
//				TargetingPawn = TargetPawn;
//				bTargeting = true;
//				LockOnAddViewport(true);
//				_DEBUG("Lock True");
//			}
//		}
//
//		return;
//	}
//}
//
//void AARPGUnit::LockOnSetPosition(FVector TargetPos)
//{
//	if (OwnerController)
//	{
//		FVector2D Pos;
//		OwnerController->ProjectWorldLocationToScreen(TargetPos, Pos);
//		OwnerController->SetLockPosition(Pos);
//	}
//}

//void AARPGUnit::LockOnAddViewport(bool bFlag)
//{
//	OwnerController->LockOnAddViewport(bFlag);
//}


void AARPGUnit::PlaySound(class USoundBase* Sound)
{
	if (OwnerController)
		OwnerController->SetPlaySound(Sound);
}







