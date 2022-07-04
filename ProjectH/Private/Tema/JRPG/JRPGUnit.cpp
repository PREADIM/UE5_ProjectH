// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGComponent.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGGameMode.h"

// Sets default values
AJRPGUnit::AJRPGUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 520.f, 0.f);

	BattleComponent = CreateDefaultSubobject<UJRPGComponent>(TEXT("BattleComponent"));


	//3��Ī�� ǥ�� �� ��������
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;


	//3��Ī ī�޶�
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera3P"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	Priority = 0;
}

// Called when the game starts or when spawned
void AJRPGUnit::BeginPlay()
{
	Super::BeginPlay();
	BattleComponent->OwnerUnit = this;


	// �ڡ� ���������� �̰��� ����ɰ��̴�, ĳ���� �ѹ��� �˻��ؼ� ������ �����´�.

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
		OwnerController->RepreCharacterNum = CharacterNum; // �ٽ� �����ؾ��ϴ� ĳ���� ����.
		OwnerController->RepreCharacter = this;
		
		//�� ��ǥ ĳ���� ������� ����� ���⼭ �ش� ĳ������ ������ ������ �ʱ�ȭ�ϴ� �۾� ����.
	}

}


/*----------------
	Move & Utils
-----------------*/
void AJRPGUnit::Forward(float Value)
{
	if(!bIsLMBAttack)
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), Value);

}


void AJRPGUnit::MoveRight(float Value)
{
	if (!bIsLMBAttack)
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

	if (PlayerType == EPlayerType::Enermy) // �÷��̾ ����.
	{
		
	}
	else if (PlayerType == EPlayerType::Player) // �÷��̾ ����.
	{
		
	}

	return DamageApplied;
}


void AJRPGUnit::LMB()
{
	if (LMBAnim)
	{
		if (bIsLMBAttack)
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

void AJRPGUnit::Skill_2()
{
	CallSkill_2();
}

void AJRPGUnit::Skill_3()
{
	CallULT();
}




// �ش� ĳ������ �� �̶�� ��.
void AJRPGUnit::BattleStart()
{
	BattleComponent->Init(); // �ش� ĳ������ ������ ��Ÿ���� ���� ���� �ʱ�ȭ.
	BattleComponent->BattleStart(); // 

	// UI�� ��� ������ �ʱ�ȭ �صΰ�, UI���� ����.
}

void AJRPGUnit::EnermyBattleStart()
{
	// ȭ�鿡�ִ� UI�� ����ȭ.
	// ���� ���� �ൿ ����.
	// ���� ���� �� ĳ���� Ÿ�� ����.
	// Ÿ���� �������� �ش� Ÿ������ ī�޶� �̵�. 
	// (�ش� ĳ���Ͱ� �� ���� �����־���ϹǷ�, �� ���� ���ؼ� ī�޶� ȸ���ؾ��Ѵ�.)
	// (FindLookAtRotation) �� Ȱ���ϸ� �ɵ�.
	// �� �ൿ�� ����.
}

