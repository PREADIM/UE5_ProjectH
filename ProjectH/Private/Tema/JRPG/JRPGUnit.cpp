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

	//3인칭을 표현 할 스프링암
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;


	//3인칭 카메라
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
	// ★★ 스폰했을때 이것이 실행될것이니, 캐릭터 넘버로 검색해서 스탯을 가져온다.
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

	if (bIsJRPGUnit) // 턴제 유닛으로 소환된 경우.
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
			OwnerController->RepreCharacterNum = CharNum; // 다시 빙의해야하는 캐릭터 저장.
			OwnerController->RepreCharacter = this;

			//★ 대표 캐릭터 변경로직 실행시 여기서 해당 캐릭터의 정보를 위젯에 초기화하는 작업 수행.
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
			// 캐릭터 생사여부를 위한 지우기. 적이 공격할 아군을 찾기위한 배열.
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
			// 적 선택 리스트 UI를 위해 지우기.
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
		PlayAnimMontage(LMBAnim); // 배틀 시작 공격 애니메이션 실행.
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




// 해당 캐릭터의 턴 이라는 것. true는 오너, false는 적이다.
void AJRPGUnit::BattleStart(bool bFlag) // 이 함수는 아직 위젯을 보이게 하기전에 먼저 위젯 아이콘을 미리 셋팅해두는 함수.
{
	// BattleComponent를 안쓰는 방법.
	if (OwnerController)
	{
		OwnerController->BattleTurnStart(bFlag);
	}

	// UI에 모든 정보를 초기화 해두고, UI에서 실행.
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
	OwnerController->SetEnermyTurnWidget(true); // 적의 차례니까 위젯을 필요한것만 남긴다.

	if (!OwnerAIController)
	{
		_DEBUG("Not AI Controller");
		return;
	}


	OwnerAIController->SetIsTurn(true); // 턴이다.


	// 적이 취할 행동 설정.
	// 적이 때릴 내 캐릭터 타겟 설정.
	// 타겟을 정했으면 해당 타겟으로 카메라 이동. 
	// (해당 캐릭터가 그 적을 보고있어야하므로, 그 적을 향해서 카메라가 회전해야한다.)
	// (FindLookAtRotation) 을 활용하면 될듯.
	// 적 행동을 실행.
	// BT로 구현
}


void AJRPGUnit::InitCurrentStat()
{
	CurrentHP = CharacterStat.MaxHP;
	CurrentMP = CharacterStat.MaxMP;
	Priority = CharacterStat.Priority;
	MaxULTGage = CharacterStat.MaxULT;
	BattleHPWidget->Init();

	//ULTGage = MaxULTGage; // ★★ 궁극기 테스트
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
	Unit->CurrentMP = FMath::Clamp(Unit->CurrentMP + 10.f, 0.0f, Unit->CharacterStat.MaxMP); // 맞은 유닛은 마나가 차게한다.
	Unit->ULTGage = FMath::Clamp(Unit->ULTGage + 10.f, 0.0f, Unit->MaxULTGage); // 맞은 유닛은 궁게가 차게한다.
}

void AJRPGUnit::AddManyMPAndULT()
{
	for (AJRPGUnit* Unit : OwnerController->TargetUnits)
	{
		Unit->CurrentMP = FMath::Clamp(Unit->CurrentMP + 10.f, 0.0f, Unit->CharacterStat.MaxMP); // 맞은 유닛은 마나가 차게한다.
		Unit->ULTGage = FMath::Clamp(Unit->ULTGage + 10.f, 0.0f, Unit->MaxULTGage); // 맞은 유닛은 궁게가 차게한다.
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