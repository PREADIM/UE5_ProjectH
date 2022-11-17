// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGCamera.h"
#include "Tema/JRPG/JRPGComponent.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGAIController.h"
#include "Components/WidgetComponent.h"
#include "Tema/JRPG/BattleUI/JRPGBattleHPWidget.h"
#include "Tema/JRPG/JRPGCamera.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AJRPGUnit::AJRPGUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 520.f, 0.f);

	//3��Ī�� ǥ�� �� ��������
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;


	//3��Ī ī�޶�
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera3P"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	BattleHPComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("BattleHP"));
	BattleHPComponent->SetupAttachment(RootComponent);
	BattleHPComponent->SetWidgetSpace(EWidgetSpace::Screen);
	BattleHPComponent->SetDrawSize(FVector2D(150.f, 35.f));

	Priority = 0;

	bIsLMBAttack = false;
}

// Called when the game starts or when spawned
void AJRPGUnit::BeginPlay()
{
	Super::BeginPlay();
	// �ڡ� ���������� �̰��� ����ɰ��̴�, ĳ���� �ѹ��� �˻��ؼ� ������ �����´�.
	GM = Cast<AJRPGGameMode>(GetWorld()->GetAuthGameMode());
	BattleHPWidget = Cast<UJRPGBattleHPWidget>(BattleHPComponent->GetUserWidgetObject());
	if (BattleHPWidget)
	{
		BattleHPWidget->OwnerUnit = this;
		BattleWidgetOnOff(false);
	}

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

			//�� ��ǥ ĳ���� ������� ����� ���⼭ �ش� ĳ������ ������ ������ �ʱ�ȭ�ϴ� �۾� ����.
		}
	}
	

}


/*----------------
	Move & Utils
-----------------*/
void AJRPGUnit::Forward(float Value)
{
	if(!bIsLMBAttack && OwnerController->GameType != EGameModeType::UI)
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), Value);

}


void AJRPGUnit::MoveRight(float Value)
{
	if (!bIsLMBAttack && OwnerController->GameType != EGameModeType::UI)
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), Value);

}


void AJRPGUnit::LookUp(float AxisValue)
{
	//AddControllerPitchInput(AxisValue * MouseSensitivity * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(AxisValue);
}


void AJRPGUnit::LookRight(float AxisValue)
{
	//AddControllerYawInput(AxisValue * MouseSensitivity * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(AxisValue);
}


// Called to bind functionality to input
void AJRPGUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &AJRPGUnit::Forward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJRPGUnit::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AJRPGUnit::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AJRPGUnit::LookRight);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AJRPGUnit::LMB);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("TestKey", IE_Pressed, this, &AJRPGUnit::TestKey);
}



float AJRPGUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	float DFEDamage = 100 / (100 + CharacterStat.Shelid);
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



void AJRPGUnit::LMB()
{
	if (LMBAnim)
	{
		if (bIsLMBAttack || OwnerController->GameType == EGameModeType::UI)
			return;

		CallLMB();
		PlayAnimMontage(LMBAnim); // ��Ʋ ���� ���� �ִϸ��̼� ����.
		bIsLMBAttack = true;
	}
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
	// BattleComponent�� �Ⱦ��� ���.
	if (OwnerController)
	{
		OwnerController->BattleTurnStart(bFlag);
	}

	// UI�� ��� ������ �ʱ�ȭ �صΰ�, UI���� ����.
}

void AJRPGUnit::OwnerUnitBattleStart()
{
	if (!OwnerController)
		return;

	OwnerController->CameraSetUp(GetActorLocation());
	OwnerController->EnermyListSetup();
	OwnerController->SetEnermyTurnWidget(false);
	OwnerController->SetVisibleBattleWidget(true);
}

void AJRPGUnit::EnermyBattleStart()
{
	if (!OwnerController)
		return;

	OwnerController->SetVisibleBattleWidget(true);
	OwnerController->SetEnermyTurnWidget(true); // ���� ���ʴϱ� ������ �ʿ��Ѱ͸� �����.

	if (!OwnerAIController)
	{
		_DEBUG("Not AI Controller");
		return;
	}


	OwnerAIController->SetIsTurn(true); // ���̴�.


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
	BattleHPWidget->Init();

	//ULTGage = MaxULTGage; // �ڡ� �ñر� �׽�Ʈ
}

void AJRPGUnit::TargetAttack(float ATK)
{
	if (OwnerController)
	{
		FDamageEvent DamageEvent;
		OwnerController->TargetUnit->TakeDamage(ATK, DamageEvent, OwnerController, this);
	}

}

void AJRPGUnit::TargetManyAttack(float ATK)
{
	if (OwnerController)
	{
		for (AJRPGUnit* Unit : OwnerController->TargetUnits)
		{
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
	{
		BattleHPWidget->SetRenderOpacity(1.0f);
	}
	else
	{
		BattleHPWidget->SetRenderOpacity(0.0f);
	}
}

void AJRPGUnit::AddMPAndULT()
{
	AJRPGUnit* Unit = OwnerController->TargetUnit;
	Unit->CurrentMP = FMath::Clamp(Unit->CurrentMP + 10.f, 0.0f, Unit->CharacterStat.MaxMP); // ���� ������ ������ �����Ѵ�.
	Unit->ULTGage = FMath::Clamp(Unit->ULTGage + 10.f, 0.0f, Unit->MaxULTGage); // ���� ������ �ð԰� �����Ѵ�.
}

void AJRPGUnit::AddManyMPAndULT()
{
	for (AJRPGUnit* Unit : OwnerController->TargetUnits)
	{
		Unit->CurrentMP = FMath::Clamp(Unit->CurrentMP + 10.f, 0.0f, Unit->CharacterStat.MaxMP); // ���� ������ ������ �����Ѵ�.
		Unit->ULTGage = FMath::Clamp(Unit->ULTGage + 10.f, 0.0f, Unit->MaxULTGage); // ���� ������ �ð԰� �����Ѵ�.
	}	
}


void AJRPGUnit::OwnerAddMPAndULT()
{
	CurrentMP = FMath::Clamp(CurrentMP + 20.f, 0.0f, CharacterStat.MaxMP);
	ULTGage = FMath::Clamp(ULTGage + 20.f, 0.0f, MaxULTGage);
}

void AJRPGUnit::OwnerAddULT()
{
	ULTGage = FMath::Clamp(ULTGage + 20.f, 0.0f, MaxULTGage);
}

void AJRPGUnit::UnitTurnEnd()
{
	if (OwnerController)
	{
		OwnerController->UnitTurnEnd();
	}
}


void AJRPGUnit::AttackEnd()
{
	OwnerController->SetVisibleBattleWidget(false);
}



void AJRPGUnit::CallAIAttackEnd()
{
	OnAIAttackEnd.Broadcast();
}