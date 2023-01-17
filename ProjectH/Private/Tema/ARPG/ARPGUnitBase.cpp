// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGUnitBase.h"
#include "Components/WidgetComponent.h"
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

	UnitState.Init(this);
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

float AARPGUnitBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamage.Broadcast(DamageAmount);

	if (BattleHP->GetRenderOpacity() == 0.f)
	{
		BattleHP->SetRenderOpacity(1.f);
	}

	return DamageAmount;
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

