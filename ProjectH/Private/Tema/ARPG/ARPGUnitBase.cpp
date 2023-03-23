// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGUnitBase.h"
#include "Components/WidgetComponent.h"
#include "Tema/ARPG/Widget/ARPGWidget_BattleHP.h"
#include "GameMode/ProjectHGameInstance.h"


// Sets default values
AARPGUnitBase::AARPGUnitBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BattleHPComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
	BattleHPComponent->SetupAttachment(RootComponent);
	BattleHPComponent->SetDrawSize(FVector2D(185.f, 55.f));
	BattleHPComponent->SetWidgetSpace(EWidgetSpace::Screen);
	BattleHPComponent->SetOwnerNoSee(true);

	APSpeed = 50.f;
	APUseSpeed = 30.f;
	CurrentAttackFactor = 1.0f;
	SurfaceDistance = 300.f;
}

// Called when the game starts or when spawned
void AARPGUnitBase::BeginPlay()
{
	Super::BeginPlay();

	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	BattleHP = Cast<UARPGWidget_BattleHP>(BattleHPComponent->GetUserWidgetObject());
	if (BattleHP)
	{
		BattleHP->Init(this);
		BattleHP->SetRenderOpacity(0.f);
		BattleHPComponent->SetOwnerNoSee(true);
		// ��Ƽ �����̸� SetOwnerNoSee�� �ϸ����������?
	}
}

// Called every frame
void AARPGUnitBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetPhysicalSound();

	if (!bZeroAP && !bAttackAndHitAP)
	{
		if (!bUseAP)
		{
			UnitState.SetAP(UnitState.AP + (APSpeed * DeltaSeconds));
		}
		else
		{
			if (bUsingAP) // �����ؼ� ����ϴ� �ൿ, ��ó� ��¡ ����
			{
				UnitState.SetAP(UnitState.AP - (APUseSpeed * DeltaSeconds));
				if (UnitState.AP <= 0.f)
				{
					ZeroAP();
				}
			}
		}
	}
	else
	{
		if (bZeroAP)
		{
			if (CurrentWaitTime >= ZeroAPWaitTime)
			{
				bZeroAP = false;
				CurrentWaitTime = 0.f;
				bAttackAndHitAP = false; // �̰͵� ���� ������Ѵ�.
			}
			else
			{
				CurrentWaitTime = CurrentWaitTime + DeltaSeconds;
			}
		}
		else if (bAttackAndHitAP)
		{
			if (CurrentWaitTime >= AttackAPWaitTime)
			{
				bAttackAndHitAP = false;
				CurrentWaitTime = 0.f;
			}
			else
			{
				CurrentWaitTime = CurrentWaitTime + DeltaSeconds;
			}
		}

		
	}


}


bool AARPGUnitBase::Hit(bool bBlockingHit)
{
	// ���ε��� 0���ϸ� ��Ʈ ���
	return UnitState.Poise <= 0;
}

void AARPGUnitBase::ZeroAP()
{
	bUseAP = false;
	bUsingAP = false;
	bZeroAP = true;
}


void AARPGUnitBase::Death()
{
	bDeath = true;
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AARPGUnitBase::BattleHPWidgetHide, 4.0f, false);
	/*GetWorld()->GetTimerManager().SetTimer(DeathHandle, FTimerDelegate::CreateLambda([&]()
	{
		Destroy();		
	}), 20.f, false);*/
	// �׾����� ���� �Լ��� �ϴ� ����. �̷ο��� ��ã���� ǥ�Ŀ����� ��밡��

	BPBindDeath();
}

void AARPGUnitBase::BattleHPWidgetHide()
{ 
	BattleHPComponent->SetVisibility(false); 
}

// Called to bind functionality to input
void AARPGUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


float AARPGUnitBase::TakeDamageCalculator(AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser)
{
	return 0.f;
}

float AARPGUnitBase::DamageFunction(AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser)
{
	// ���ε� ���
	// ���ε��� �ð��� ������ ���� �Ͱ� Ư�� �ൿ�� �ϸ� �ٽ� ���� �� �������� ��������.

	// ���ε��� 0 �Ʒ���� ���� ���� �Ծ��ٴ� �� ������ �ٽ� �ʱ�ȭ.
	// ���⼭ ������ ��������� ���ε��� �׳� ���� ������ ����.
	if (UnitState.Poise <= 0)
		UnitState.ResetPoise();

	if (DamageCauser)
	{
		// �⺻ ���� NoramllPoise�� �����Ѵ�. Poise�� ��Ӻ��ϴ� �����̱⶧��.
		UnitState.Poise -= DamageCauser->UnitState.NormallyPoise;
	}

	return 0.0f;
}


float AARPGUnitBase::CalculDamage(float Damage)
{
	return (Damage + UnitState.ATK) * CurrentAttackFactor;
}

float AARPGUnitBase::CalculAPDamage(float APDamage)
{
	return APDamage + CurrentAP_DMG;
}


bool AARPGUnitBase::CanUseAP()
{
	if (bParringHit)
		return false;

	return UnitState.AP >= 10.f ? true : false;
}

bool AARPGUnitBase::ParringHitFunc(FVector TargetLocation)
{
	return TargetDotProduct(TargetLocation, 0.76); // �� 40��
}


bool AARPGUnitBase::TargetDotProduct(FVector TargetLocation, float CompareCos)
{
	FVector NormalTargetV = TargetLocation - GetActorLocation();
	NormalTargetV.Z *= 0.f; // Z�� �Ұ�
	NormalTargetV.Normalize();

	float Dot = FVector::DotProduct(GetActorForwardVector(), NormalTargetV);

	if (CompareCos <= Dot)
		return true;
	else
		return false;
}

void AARPGUnitBase::SetPhysicalSound()
{
	TEnumAsByte<EPhysicalSurface> PS = TracePysicalSurface(this, SurfaceDistance);

	if (!PhysicalAllSounds.Find(PS))
		return;

	PhysicalSounds = PhysicalAllSounds[PS]; // �ش��ϴ� ǥ���� ���� ��������

}
