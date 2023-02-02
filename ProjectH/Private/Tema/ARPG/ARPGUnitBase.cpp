// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGUnitBase.h"
#include "Components/WidgetComponent.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "Tema/ARPG/Widget/ARPGWidget_BattleHP.h"

// Sets default values
AARPGUnitBase::AARPGUnitBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BattleHPComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
	BattleHPComponent->SetupAttachment(RootComponent);
	BattleHPComponent->SetDrawSize(FVector2D(150.f, 35.f));
	BattleHPComponent->SetWidgetSpace(EWidgetSpace::Screen);
	BattleHPComponent->SetOwnerNoSee(true);

	APSpeed = 50.f;
	APUseSpeed = 30.f;
	CurrentAttackFactor = 1.0f;
}

// Called when the game starts or when spawned
void AARPGUnitBase::BeginPlay()
{
	Super::BeginPlay();
	BattleHP = Cast<UARPGWidget_BattleHP>(BattleHPComponent->GetUserWidgetObject());
	if (BattleHP)
	{
		BattleHP->Init(this);
		BattleHP->SetRenderOpacity(0.f);
		//BattleHPComponent->SetOwnerNoSee(true);
		// ��Ƽ �����̸� SetOwnerNoSee�� �ϸ����������?
	}
}

// Called every frame
void AARPGUnitBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (!bZeroAP)
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
		if (CurrentWaitTime >= ZeroAPWaitTime)
		{
			bZeroAP = false;
			CurrentWaitTime = 0.f;
		}
		else
		{
			CurrentWaitTime = CurrentWaitTime + DeltaSeconds;
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
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AARPGUnitBase::BattleHPWidgetHide, 5.0f, false);
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

//float AARPGUnitBase::TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	if (DamageAmount > 0.f)
//	{
//		OnDamage.Broadcast(DamageAmount);
//	}
//
//	return DamageAmount;
//}

float AARPGUnitBase::TakeDamageCalculator(AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser)
{
	// ���ε� ���
	// ���ε� �� �ð��� ������ ���� �Ͱ� Ư�� �ൿ�� �ϸ� �ٽ� ���� �� �������� ��������.

	// ���ε��� 0 �Ʒ���� ���� ���� �Ծ��ٴ� �� ������ �ٽ� �ʱ�ȭ.
	if (UnitState.Poise <= 0)
		UnitState.ResetPoise();

	if (DamageCauser)
	{
		// �⺻ ���� NoramllPoise�� �����Ѵ�. Poise�� ��Ӻ��ϴ� �����̱⶧��.
		UnitState.Poise -= DamageCauser->UnitState.NormallyPoise;
	}

	return 0.f;
}


float AARPGUnitBase::CalculDamage(float Damage)
{
	float HPDMG = (Damage + UnitState.ATK) * CurrentAttackFactor;

	return HPDMG;
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
