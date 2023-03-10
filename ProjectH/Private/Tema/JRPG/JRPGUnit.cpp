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

	OverlapBattleStartCollision->OnComponentBeginOverlap.AddDynamic(this, &AJRPGUnit::BattleStartCollisionBeginOverlap);
}

// Called every frame
void AJRPGUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetPhysicalSound();

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
	if (OwnerController->GameType != EGameModeType::UI)
	{
		FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
		Direction.Z = 0.f;
		Direction.Normalize();
		AddMovementInput(Direction, Value);
	}
}


void AJRPGUnit::MoveRight(float Value)
{
	if (OwnerController->GameType != EGameModeType::UI)
	{
		FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
		Direction.Z = 0.f;
		Direction.Normalize();
		AddMovementInput(Direction, Value);
	}
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



float AJRPGUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	

	float DFEDamage = 100 / (100 + CharacterStat.Shield);
	float Damage = DamageAmount * DFEDamage;

	if (CurrentHP <= Damage)
	{
		CurrentHP = 0;
		
		if (PlayerType == EPlayerType::Player)
		{
			// ĳ���� ���翩�θ� ���� �����. ���� ������ �Ʊ��� ã������ �迭.
			TArray<AJRPGUnit*>& OwnerList = GM->OwnerList;
			for (int32 i = 0; i < OwnerList.Num(); i++)
			{
				if (OwnerList[i] == this)
				{
					OwnerList.RemoveAt(i);
					break;
				}
			}
		}
		else
		{
			// �� ���� ����Ʈ UI�� ���� �����.
			TArray<AJRPGUnit*>& EnermyList = GM->EnermyList;
			for (int32 i = 0; i < EnermyList.Num(); i++)
			{
				if (EnermyList[i] == this)
				{
					EnermyList.RemoveAt(i);
				}
			}

		}

		TArray<FPriorityUnit>& SetUnitList = GM->SetUnitList;
		for (int32 i = 0; i < SetUnitList.Num(); ++i)
		{
			if (SetUnitList[i].Unit == this)
			{
				SetUnitList.RemoveAt(i);
			}
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

	return DamageApplied;
}

void AJRPGUnit::NormalAttack()
{
	CallNormalAttack();
}

void AJRPGUnit::Skill_1()
{
	CallSkill_1();
}


void AJRPGUnit::Skill_ULT()
{
	CallULT();
}


// �ش� ĳ������ �� �̶�� ��. true�� ����, false�� ���̴�.
void AJRPGUnit::BattleStart(bool bFlag) // �� �Լ��� ���� ������ ���̰� �ϱ����� ���� ���� �������� �̸� �����صδ� �Լ�.
{
	if (OwnerController)
		OwnerController->BattleTurnStart(bFlag);
	// UI�� ��� ������ �ʱ�ȭ �صΰ�, UI���� ����.
}

void AJRPGUnit::OwnerUnitBattleStart()
{
	if (!OwnerController)
		return;

	OwnerController->CameraSetUp(GetActorLocation());

	if (bCC) // CC�� ������ ��� ��ŵ
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
		{
			UnitTurnEnd();
		}), 3.f, false);
	}
	else
	{
		OwnerController->SetEnermyTurnWidget(false); // �� ���ʽ� ���̸� �ȵǾ��� ��ų ��ư �� �� ���� ����Ʈ ���̰� �ϱ�.
	}
	
	OwnerController->SetVisibleBattleWidget(true); // ���� ���̱�
	OwnerController->EnermyListSetup();	
}




void AJRPGUnit::EnermyBattleStart()
{
	// �� �Լ��� ó���ϴ� ������ ���̴�. �÷��̾ �ƴϴ�.
	// ���� �÷��̾��� ��Ʈ�ѷ��� �������ִ�. ���Ӹ�忡�� �����Ҷ� ���� �޾ƿ�.
	if (!OwnerController)
		return;

	OwnerController->SetEnermyTurnWidget(true); // ���� ���ʴϱ� ������ �ʿ��Ѱ͸� �����.
	OwnerController->SetVisibleBattleWidget(true); // ���� ���̱�
	
	if (!OwnerAIController)
		return;

	if (bCC) // CC�� ������ ��� ��ŵ
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
			{
				UnitTurnEnd();
			}), 3.f, false);
	}
	else
	{
		OwnerAIController->SetIsTurn(true); // ���̴�.
	}
	
	// ���� ���� �ൿ ����.
	// ���� ���� �� ĳ���� Ÿ�� ����.
	// Ÿ���� �������� �ش� Ÿ������ ī�޶� �̵�. 
	// (�ش� ĳ���Ͱ� �� ���� �����־���ϹǷ�, �� ���� ���ؼ� ī�޶� ȸ���ؾ��Ѵ�.)
	// (FindLookAtRotation) �� Ȱ���ϸ� �ɵ�.
	// �� �ൿ�� ����.
	// BT�� ����
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
				FDebuffStruct DebuffStruct(CharacterName, DebuffClass);
				if (!OwnerController->TargetUnit->DebuffSet.Find(DebuffStruct))
				{
					OwnerController->TargetUnit->DebuffSet.Emplace(DebuffStruct);
					DebuffClass->DebuffFunction(OwnerController->TargetUnit);
					OwnerController->TargetUnit->BattleHPWidget->SetBuffIcon();
				}
			}
		}

		FDamageEvent DamageEvent;
		OwnerController->TargetUnit->TakeDamage(ATK, DamageEvent, OwnerController, this);
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
					}

				}

			}

			FDamageEvent DamageEvent;
			Unit->TakeDamage(ATK, DamageEvent, OwnerController, this);
		}
	}

}


void AJRPGUnit::ThisUnitBattleUnit(bool bFlag)
{
	if (bFlag)
		BattleWidgetOnOff(true);
	else
		BattleWidgetOnOff(false);
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

void AJRPGUnit::AddMPAndULT()
{
	AJRPGUnit* Unit = OwnerController->TargetUnit;
	Unit->CurrentMP = FMath::Clamp(Unit->CurrentMP + 10.f, 0.0f, Unit->CharacterStat.MaxMP); // ���� ������ ������ �����Ѵ�.
	Unit->CurrentULTGage = FMath::Clamp(Unit->CurrentULTGage + 10.f, 0.0f, Unit->MaxULTGage); // ���� ������ �ð԰� �����Ѵ�.
}

void AJRPGUnit::AddManyMPAndULT()
{
	for (AJRPGUnit* Unit : OwnerController->TargetUnits)
	{
		Unit->CurrentMP = FMath::Clamp(Unit->CurrentMP + 10.f, 0.0f, Unit->CharacterStat.MaxMP); // ���� ������ ������ �����Ѵ�.
		Unit->CurrentULTGage = FMath::Clamp(Unit->CurrentULTGage + 10.f, 0.0f, Unit->MaxULTGage); // ���� ������ �ð԰� �����Ѵ�.
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

void AJRPGUnit::UnitTurnEnd()
{
	if (OwnerController)
	{
		for (FDebuffStruct& Debuff : DebuffSet)
		{
			if (Debuff.DebuffClass->SetupCnt())
			{
				Debuff.DebuffClass->DebuffTurnEndFunction(this);
				DebuffSet.Remove(Debuff);
			}
			BattleHPWidget->SetBuffIcon();
		}

		OwnerController->UnitTurnEnd();
	}
}

void AJRPGUnit::AttackEnd()
{
	OwnerController->SetVisibleBattleWidget(false);
}

void AJRPGUnit::BattleStartCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodtIndex, bool bFromSweep, const FHitResult& HitResult)
{
	if (OwnerController && OwnerController->GetBattleING())
		return;

	class AJRPGEnermy* CurrentOverlapFieldEnermy = Cast<class AJRPGEnermy>(OtherActor);
	if (CurrentOverlapFieldEnermy)
		OwnerController->PlayBattleMode(CurrentOverlapFieldEnermy);
}


void AJRPGUnit::SetPhysicalSound()
{
	TEnumAsByte<EPhysicalSurface> PS = TracePysicalSurface(this, SurfaceDistance);

	if (!PhysicalAllSounds.Find(PS))
		return;

	PhysicalSounds = PhysicalAllSounds[PS]; // �ش��ϴ� ǥ���� ���� ��������

}


void AJRPGUnit::PlayStartMontage()
{
	if (AnimInstance && BattleStartMontage)
	{
		AnimInstance->Montage_Play(BattleStartMontage);
	}
}


void AJRPGUnit::SetCCState(ECCType CCType, bool bFlag)
{
	if (bFlag)
		bCC = true; // �̰� CC �ִϸ��̼ǵ��� ������ END�� ��Ƽ���̷� �� ����.

	switch (CCType)
	{
	case ECCType::NONE:
		break;
	case ECCType::STUN:
		CCState.bStun = bFlag;
		break;
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