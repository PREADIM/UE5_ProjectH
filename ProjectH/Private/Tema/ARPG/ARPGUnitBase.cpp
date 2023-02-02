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
		// 멀티 게임이면 SetOwnerNoSee를 하면되지않을까?
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
			if (bUsingAP) // 지속해서 사용하는 행동, 대시나 차징 공격
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
	// 강인도가 0이하면 히트 모션
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
	// 강인도 계산
	// 강인도 를 시간이 지나면 차는 것과 특정 행동을 하면 다시 차게 할 순있지만 하지않음.

	// 강인도가 0 아래라는 것은 경직 먹었다는 뜻 때문에 다시 초기화.
	if (UnitState.Poise <= 0)
		UnitState.ResetPoise();

	if (DamageCauser)
	{
		// 기본 값인 NoramllPoise를 빼야한다. Poise는 계속변하는 변수이기때문.
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
	return TargetDotProduct(TargetLocation, 0.76); // 약 40도
}


bool AARPGUnitBase::TargetDotProduct(FVector TargetLocation, float CompareCos)
{
	FVector NormalTargetV = TargetLocation - GetActorLocation();
	NormalTargetV.Z *= 0.f; // Z축 소거
	NormalTargetV.Normalize();

	float Dot = FVector::DotProduct(GetActorForwardVector(), NormalTargetV);

	if (CompareCos <= Dot)
		return true;
	else
		return false;
}
