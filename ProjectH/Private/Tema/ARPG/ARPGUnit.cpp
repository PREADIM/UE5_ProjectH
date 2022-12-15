// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGUnit.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Tema/ARPG/ARPGPlayerController.h"
#include "Tema/ARPG/ARPGWeapon.h"
#include "Tema/ARPG/ARPGShield.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetmathLibrary.h"

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

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> SwordM(TEXT(""));
	if (SwordM.Succeeded())
	{
		SwordMesh->SetStaticMesh(SwordM.Object);
	}*/

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

	if (BP_Sword && BP_Shield)
	{
		Sword = GetWorld()->SpawnActor<AARPGWeapon>(BP_Sword);
		Shield = GetWorld()->SpawnActor<AARPGShield>(BP_Shield);
		
		if (Sword && Shield)
		{
			Sword->OwnerUnit = this;
			Shield->OwnerUnit = this;
			Sword->SetOwner(this);
			Shield->SetOwner(this);

			Sword->AttachToComponent(FPSMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("IdleSword"));
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
	AddMovementInput(GetActorForwardVector(), Value);

}

void AARPGUnit::MoveRight(float Value)
{
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), Value);
	AddMovementInput(GetActorRightVector(), Value);
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
	if (!bBlocking)
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
	bBlocking = true;
	GetCharacterMovement()->MaxWalkSpeed = BlockSpeed;
}

void AARPGUnit::RMBReleased()
{
	bBlocking = false;
	bParring = false;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AARPGUnit::Sprint()
{
	if (bBlocking)
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
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AARPGUnit::Parring()
{
	if(bBlocking && !bParring)
		bParring = true;

}

// 적 락온
void AARPGUnit::LockOn()
{
	if (bTargeting)
	{
		bTargeting = false;
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

float AARPGUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}








