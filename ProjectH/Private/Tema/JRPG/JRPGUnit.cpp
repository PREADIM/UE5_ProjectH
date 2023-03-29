// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGCamera.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGAIController.h"
#include "Components/WidgetComponent.h"
#include "Tema/JRPG/BattleUI/JRPGBattleHPWidget.h"
#include "Tema/JRPG/JRPGCamera.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Animation/AnimInstance.h"



// Sets default values
AJRPGUnit::AJRPGUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 520.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera3P"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	OverlapBattleStartCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapBattleStartCollision"));
	OverlapBattleStartCollision->SetupAttachment(RootComponent);
	OverlapBattleStartCollision->SetSphereRadius(150.f);

	BattleHPComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("BattleHP"));
	BattleHPComponent->SetupAttachment(RootComponent);
	BattleHPComponent->SetWidgetSpace(EWidgetSpace::Screen);
	BattleHPComponent->SetDrawSize(FVector2D(190.f, 55.f));
	BattleHPComponent->SetDrawAtDesiredSize(true);
	BattleHPComponent->SetTickMode(ETickMode::Automatic);

	Priority = 0;
}

void AJRPGUnit::BeginPlay()
{
	Super::BeginPlay();
	// �ڡ� ���������� �̰��� ����ɰ��̴�, ĳ���� �ѹ��� �˻��ؼ� ������ �����´�.
	GM = Cast<AJRPGGameMode>(GetWorld()->GetAuthGameMode());
	BattleHPWidget = Cast<UJRPGBattleHPWidget>(BattleHPComponent->GetUserWidgetObject());
	if (BattleHPWidget)
	{
		//BattleHPWidget->OwnerUnit = this;
		BattleWidgetOnOff(false);
	}

	UProjectHGameInstance* GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
		MouseSensitivity = GI->MS;

	BattleDefaultLocation = GetActorLocation();

	OverlapBattleStartCollision->OnComponentBeginOverlap.AddDynamic(this, &AJRPGUnit::BattleStartCollisionBeginOverlap);
}

// Called every frame
void AJRPGUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsJRPGUnit) // ���� �������� ��ȯ�� ���.
	{
		if (OwnerController->TargetUnit)
		{
			FVector V = OwnerController->DynamicCamera->GetActorLocation() - GetActorLocation();
			float Range = UKismetMathLibrary::MapRangeUnclamped(V.Length(), 100.f, 600.f, 1.f, 0.8f);
			BattleHPWidget->SetRenderScale(FVector2D(Range, Range));
		}
	}
}

void AJRPGUnit::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(Cast<AJRPGPlayerController>(NewController))
	{
		OwnerController = Cast<AJRPGPlayerController>(NewController);
		if (OwnerController)
		{
			OwnerController->RepreCharacterNum = CharNum; // �ٽ� �����ؾ��ϴ� ĳ���� ����.
			OwnerController->RepreCharacter = this;
		}
	}
}

void AJRPGUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
}


/*----------------
	Move & Utils
-----------------*/
void AJRPGUnit::Forward(float Value)
{
	FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
	Direction.Z = 0.f;
	Direction.Normalize();
	AddMovementInput(Direction, Value);
}


void AJRPGUnit::MoveRight(float Value)
{
	
	FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
	Direction.Z = 0.f;
	Direction.Normalize();
	AddMovementInput(Direction, Value);
}


void AJRPGUnit::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue * MouseSensitivity * GetWorld()->GetDeltaSeconds());
	//AddControllerPitchInput(AxisValue);
}


void AJRPGUnit::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue * MouseSensitivity * GetWorld()->GetDeltaSeconds());
	//AddControllerYawInput(AxisValue);
}


// Called to bind functionality to input
void AJRPGUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &AJRPGUnit::Forward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJRPGUnit::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AJRPGUnit::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AJRPGUnit::LookRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("TestKey", IE_Pressed, this, &AJRPGUnit::TestKey);
}

void AJRPGUnit::TakeDamageCalculator(float DamageAmount)
{
	float DFEDamage = 100 / (100 + CharacterStat.Shield);
	float Damage = DamageAmount * DFEDamage;

	if (CurrentHP <= Damage)
	{
		CurrentHP = 0;	

		//������ �÷��̾ ������ �˾Ƽ� ���Ӹ���� ����Ʈ���� �����ϱ�.
		DeadBattleListRemove();

		TArray<FPriorityUnit>& SetUnitList = GM->SetUnitList;
		for (int32 i = 0; i < SetUnitList.Num(); ++i)
		{
			if (SetUnitList[i].Unit == this)
				SetUnitList.RemoveAt(i);
		}

		BattleWidgetOnOff(false);
		GetCharacterMovement()->DisableMovement();
		PlayAnimMontage(DeadAnim);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	}
	else
	{
		CurrentHP -= Damage;
		PlayAnimMontage(HitAnim);
	}	
	OwnerController->VisibleDamage(Damage, GetActorLocation());

}


void AJRPGUnit::NormalAttack()
{
	InitSFX();
	AttackEnd();
	OwnerAddMPAndULT();
	CallNormalAttack();
}

void AJRPGUnit::Skill_1()
{
	InitSFX();
	AttackEnd();
	CurrentMP = FMath::Clamp(CurrentMP - UnitSkills.Skill_1.CostMP, 0.f, CharacterStat.MaxMP);
	OwnerAddULT();
	CallSkill_1();
}


void AJRPGUnit::Skill_ULT()
{
	InitSFX();
	AttackEnd();
	CurrentULTGage = 0.f;
	CallULT();
}


void AJRPGUnit::InitCurrentStat()
{
	CurrentHP = CharacterStat.MaxHP;
	CurrentMP = CharacterStat.MaxMP;
	Priority = CharacterStat.Priority;
	MaxULTGage = CharacterStat.MaxULT;
	BattleHPWidget->Init(this);
}

void AJRPGUnit::TargetAttack(float ATK, TSubclassOf<UDebuffClass> BP_DebuffClass)
{
	if (OwnerController)
	{
		if (BP_DebuffClass)
		{
			class UDebuffClass* DebuffClass = NewObject<UDebuffClass>(this, BP_DebuffClass);
			if (DebuffClass)
			{
				FDebuffStruct DebuffStruct(CharacterName, DebuffClass); // �ߺ� ������ ���� ������ ĳ���� �̸����� ����.
				if (!OwnerController->TargetUnit->DebuffSet.Find(DebuffStruct))
				{
					OwnerController->TargetUnit->DebuffSet.Emplace(DebuffStruct);
					DebuffClass->DebuffFunction(OwnerController->TargetUnit);
					OwnerController->TargetUnit->BattleHPWidget->SetBuffIcon();
					OwnerController->TargetUnit->BattleIconBuffSetup();
					
					// ���⼭ ���ָ���Ʈ�� ������ �ٲ���.
				}
			}
		}

		FDamageEvent DamageEvent;
		OwnerController->TargetUnit->TakeDamageCalculator(ATK);
	}
}

void AJRPGUnit::TargetManyAttack(float ATK, TSubclassOf<UDebuffClass> BP_DebuffClass)
{
	if (OwnerController)
	{
		for (AJRPGUnit* Unit : OwnerController->TargetUnits)
		{
			if (BP_DebuffClass)
			{
				class UDebuffClass* DebuffClass = Cast<UDebuffClass>(BP_DebuffClass->GetDefaultObject());
				if (DebuffClass)
				{
					FDebuffStruct DebuffStruct(CharacterName, DebuffClass);
					if (!Unit->DebuffSet.Find(DebuffStruct))
					{
						Unit->DebuffSet.Emplace(DebuffStruct);
						DebuffClass->DebuffFunction(Unit);
						Unit->BattleHPWidget->SetBuffIcon();
						Unit->BattleIconBuffSetup();
					}
				}
			}
			FDamageEvent DamageEvent;
			Unit->TakeDamageCalculator(ATK);
		}
	}

}

void AJRPGUnit::BattleIconBuffSetup()
{
	OwnerController->BattleIconBuffSetup(this);

}

void AJRPGUnit::UnitBattleStart()
{
	if (GetActorLocation() != BattleDefaultLocation)
		SetActorLocation(BattleDefaultLocation);
}


void AJRPGUnit::ThisUnitBattleUnit(bool bFlag)
{
	if (bFlag)
		BattleWidgetOnOff(true);
	else
		BattleWidgetOnOff(false);
	bPlayBattleStartSound = false;
	bIsJRPGUnit = bFlag;
	SetIsJRPGUnit(bFlag);
}

void AJRPGUnit::BattleWidgetOnOff(bool bOnOff)
{
	if (bOnOff)
		BattleHPWidget->SetRenderOpacity(1.0f);
	else
	{
		BattleHPWidget->SetRenderOpacity(0.0f);
	}
}


/* ���� ������ ULT �������� ������ ���� �Ѵ�.*/
void AJRPGUnit::TargetAddMPAndULT()
{
	AJRPGUnit* Unit = OwnerController->TargetUnit;
	Unit->CurrentMP = FMath::Clamp(Unit->CurrentMP + 10.f, 0.0f, Unit->CharacterStat.MaxMP);
	Unit->CurrentULTGage = FMath::Clamp(Unit->CurrentULTGage + 10.f, 0.0f, Unit->MaxULTGage);
}

void AJRPGUnit::ManyTargetsAddMPAndULT()
{
	for (AJRPGUnit* Unit : OwnerController->TargetUnits)
	{
		Unit->CurrentMP = FMath::Clamp(Unit->CurrentMP + 10.f, 0.0f, Unit->CharacterStat.MaxMP);
		Unit->CurrentULTGage = FMath::Clamp(Unit->CurrentULTGage + 10.f, 0.0f, Unit->MaxULTGage);
	}	
}


void AJRPGUnit::OwnerAddMPAndULT()
{
	CurrentMP = FMath::Clamp(CurrentMP + 20.f, 0.0f, CharacterStat.MaxMP);
	CurrentULTGage = FMath::Clamp(CurrentULTGage + 20.f, 0.0f, MaxULTGage);
}

void AJRPGUnit::OwnerAddULT()
{
	CurrentULTGage = FMath::Clamp(CurrentULTGage + 20.f, 0.0f, MaxULTGage);
}

void AJRPGUnit::MoveToAttack(float MoveSpeed, EAttackType AttackType)
{
	if (!BattleAIController)
		return;
		
	BattleAIController->ReceiveMoveCompleted.RemoveDynamic(this, &AJRPGUnit::MoveToDefaultLocationEnded);
	BattleAIController->ReceiveMoveCompleted.AddDynamic(this, &AJRPGUnit::MoveToPlayMontage);

	MoveToAttackType = AttackType;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	FVector TargetUnitLoc = OwnerController->TargetUnit->GetActorLocation();
	UAIBlueprintHelperLibrary::CreateMoveToProxyObject(this, this, TargetUnitLoc, nullptr, 300.f, false);
}

void AJRPGUnit::MoveToPlayMontage(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	AttackMontagePlay();

	/*switch (Result)
	{
	case EPathFollowingResult::Success:
		break;
	case EPathFollowingResult::Blocked:
		break;
	case EPathFollowingResult::OffPath:
		break;
	case EPathFollowingResult::Aborted:
		break;
	case EPathFollowingResult::Skipped_DEPRECATED:
		break;
	case EPathFollowingResult::Invalid:
		break;
	}*/	
}

void AJRPGUnit::AttackMontagePlay()
{
	UAnimMontage* AttackMontage = nullptr;

	switch (MoveToAttackType)
	{
	case EAttackType::Normal:
		AttackMontage = NormalAttackMontage;
		break;
	case EAttackType::Skill:
		AttackMontage = SkillMontage;
		break;
	case EAttackType::ULT:
		AttackMontage = ULTMontage;
		break;
	}

	AnimInstance->Montage_Play(AttackMontage);
	FOnMontageEnded AttackMontageEnded;
	AttackMontageEnded.BindUFunction(this, FName("MoveToDefaultLocation"));
	AnimInstance->Montage_SetEndDelegate(AttackMontageEnded, AttackMontage);
}


void AJRPGUnit::MoveToDefaultLocation()
{
	BattleAIController->ReceiveMoveCompleted.RemoveDynamic(this, &AJRPGUnit::MoveToPlayMontage);
	BattleAIController->ReceiveMoveCompleted.AddDynamic(this, &AJRPGUnit::MoveToDefaultLocationEnded);
	UAIBlueprintHelperLibrary::CreateMoveToProxyObject(this, this, BattleDefaultLocation, nullptr, 5.f, false);
}

void AJRPGUnit::MoveToDefaultLocationEnded(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
	switch (PlayerType)
	{
	case EPlayerType::Player:
		OwnerController->OwnerUnitTurnToTarget();
		break;
	case EPlayerType::Enermy:
		OwnerController->EnermyUnitTurnToTarget();
		break;
	}
	
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AJRPGUnit::UnitTurnEnd, 0.7f, false);
}


void AJRPGUnit::AttackEnd()
{
	OwnerController->SetVisibleBattleWidget(false);
}

void AJRPGUnit::TargetAttackDamageProxy(EAttackType AttackType, bool bOnce, int32 AttackCnt, TSubclassOf<class UDebuffClass> BP_DebuffClass, TSubclassOf<class UCameraShakeBase> AttackCS)
{
	float Damage = 0.f;

	switch (AttackType)
	{
	case EAttackType::Normal:
		Damage = UnitSkills.NormalAttack.SkillDamage * CharacterStat.Attack;
		break;
	case EAttackType::Skill:
		Damage = UnitSkills.Skill_1.SkillDamage * CharacterStat.Attack;
		break;
	case EAttackType::ULT:
		Damage = UnitSkills.ULT.ULTDamage * CharacterStat.Attack;
		break;
	}

	if (bOnce)
		TargetAttack(Damage / AttackCnt, BP_DebuffClass);
	else
		TargetManyAttack(Damage / AttackCnt, BP_DebuffClass);

	if (AttackCS)
		OwnerController->ClientPlayCameraShake(AttackCS, 1.f);

}


void AJRPGUnit::DamagedTurnEnd(bool bOnce, bool bTurnEnd, float Delay)
{
	if (bOnce)
		TargetAddMPAndULT();
	else
		ManyTargetsAddMPAndULT();

	/* �̵��ؼ� �����ϴ� ������ ��� �ڸ��� ���ư����ϱ� ������, �����Ḧ ���⼭ �����ʴ´�. */
	if (bTurnEnd)
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AJRPGUnit::UnitTurnEnd, Delay, false);
	}	
}


void AJRPGUnit::UnitTurnEnd()
{
	if (OwnerController)
	{
		TSet<FDebuffStruct> RemoveDebuff;
		for (FDebuffStruct& Debuff : DebuffSet)
		{
			if (Debuff.DebuffClass->SetupCnt())
			{
				Debuff.DebuffClass->DebuffTurnEndFunction(this);
				RemoveDebuff.Emplace(Debuff);
			}
		}

		/* �ݺ��߿� �����̳��� �߰��� ����� ���� ���� �����Ƿ� �̸� �޾ƿͼ� ���� */ 
		if (RemoveDebuff.Num())
		{
			for (FDebuffStruct& Debuff : RemoveDebuff)
				DebuffSet.Remove(Debuff);
		}

		BattleHPWidget->SetBuffIcon();
		OwnerController->UnitTurnEnd();
	}
}

void AJRPGUnit::BattleStartCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodtIndex, bool bFromSweep, const FHitResult& HitResult)
{
	if (OwnerController && OwnerController->GetBattleING())
		return;

	class AJRPGFieldEnermy* CurrentOverlapFieldEnermy = Cast<class AJRPGFieldEnermy>(OtherActor);
	if (CurrentOverlapFieldEnermy)
		OwnerController->PlayBattleMode(CurrentOverlapFieldEnermy);
}



void AJRPGUnit::PlayStartMontage()
{
	if (AnimInstance && BattleStartMontage)
		AnimInstance->Montage_Play(BattleStartMontage);
}

void AJRPGUnit::PlayCharacterChangeMontage()
{
	if (AnimInstance && BattleStartMontage)
		AnimInstance->Montage_Play(CharacterChangeMontage);
}


void AJRPGUnit::UnitTurnEndCCState()
{
	OwnerController->SetTurnEndDebuffWidget(CCState.LastCCType);
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AJRPGUnit::UnitTurnEnd, 2.f, false);
}

void AJRPGUnit::SetCCState(ECCType CCType, bool bFlag)
{
	if (bFlag)
		bAnimCC = true; // CC�ִϸ��̼ǵ��� ������ END�� ��Ƽ���̷� ���鼭 �ִϸ��̼� ����

	bCC = bFlag;
	CCState.SetCCType(CCType, bFlag);
}

void AJRPGUnit::InitSFX()
{
	SFXCnt = 0;
}

void AJRPGUnit::PlaySFX(EAttackType AttackType)
{
	if (AttackSFXs.Find(AttackType))
	{
		if (AttackSFXs[AttackType].AttackSounds.IsValidIndex(SFXCnt))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSFXs[AttackType].AttackSounds[SFXCnt], GetActorLocation());
			SFXCnt++;
		}		
	}
}

void AJRPGUnit::SetStatDEF(float DEF)
{
	CharacterStat.Shield = DEF;
}

void AJRPGUnit::SetStatATK(float ATK)
{
	CharacterStat.Attack = ATK;
}