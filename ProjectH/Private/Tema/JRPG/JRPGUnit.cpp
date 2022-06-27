// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGComponent.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGGameMode.h"

// Sets default values
AJRPGUnit::AJRPGUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BattleComponent = CreateDefaultSubobject<UJRPGComponent>(TEXT("BattleComponent"));

	//3인칭을 표현 할 스프링암
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;


	//3인칭 카메라
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera3P"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	Priority = 0;
}

// Called when the game starts or when spawned
void AJRPGUnit::BeginPlay()
{
	Super::BeginPlay();

	// 스폰했을때 이것이 실행될것이니, 캐릭터 넘버로 검색해서 스탯을 가져온다.

}

// Called every frame
void AJRPGUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJRPGUnit::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	OwnerController = Cast<AJRPGPlayerController>(NewController);
	if (OwnerController)
	{		
		OwnerController->RepreCharacterNum = CharacterNum; // 다시 빙의해야하는 캐릭터 저장.
		OwnerController->RepreCharacter = this;
		
		//★ 대표 캐릭터 변경로직 실행시 여기서 해당 캐릭터의 정보를 위젯에 초기화하는 작업 수행.
	}

}


/*----------------
	Move & Utils
-----------------*/
void AJRPGUnit::Forward(float Value)
{
	
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), Value);

}


void AJRPGUnit::MoveRight(float Value)
{
	
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
}


float AJRPGUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (PlayerType == EPlayerType::Enermy) // 플레이어가 때림.
	{
		AJRPGPlayerController* PC = Cast<AJRPGPlayerController>(EventInstigator);
		if (PC)
		{
			PC->PlayBattleMode(EnermyUnits);
		}
	}
	else if (PlayerType == EPlayerType::Player) // 플레이어가 맞음.
	{
		AJRPGUnit* Enermy = Cast<AJRPGUnit>(DamageCauser);
		OwnerController->PlayBattleMode(Enermy->EnermyUnits);
	}

	return DamageApplied;
}


void AJRPGUnit::LMB()
{
	if (LMBAnim)
	{
		PlayAnimMontage(LMBAnim); // 배틀 시작 공격 애니메이션 실행.

		// 컨트롤러에 현재 필드 위치정보 전달. 돌아와야하기때문.
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

void AJRPGUnit::Skill_2()
{
	CallSkill_2();
}

void AJRPGUnit::Skill_3()
{
	CallSkill_3();
}




// 해당 캐릭터의 턴 이라는 것.
void AJRPGUnit::BattleStart()
{
	BattleComponent->Init(); // 해당 캐릭터의 정보를 나타내기 위해 위젯 초기화.
	BattleComponent->BattleStart(); // 
}
