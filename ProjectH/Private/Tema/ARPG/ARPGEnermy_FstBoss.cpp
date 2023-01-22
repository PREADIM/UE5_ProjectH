// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGEnermy_FstBoss.h"

AARPGEnermy_FstBoss::AARPGEnermy_FstBoss()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARPGEnermy_FstBoss::BeginPlay()
{
	Super::BeginPlay();

}

void AARPGEnermy_FstBoss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AARPGEnermy_FstBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetMesh()) // 가끔 C++ 클래스를 상속받은 블프가 디테일창이 사라지면서 null값을 넣기때문.(버그)
	{
		FstBossAnimInstance = Cast<UARPG_FstBossAnimInstance>(GetMesh()->GetAnimInstance());
		if (FstBossAnimInstance == nullptr)
		{
			_DEBUG("Not AI Has AnimInstance");
		}
	}
}

float AARPGEnermy_FstBoss::TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamageCalculator(APDamage, DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return 0.0f;
}

void AARPGEnermy_FstBoss::TakeDamageAP(float Damage)
{
	
}

void AARPGEnermy_FstBoss::Hit()
{


}

bool AARPGEnermy_FstBoss::CanThisDamage()
{

	return false;
}

void AARPGEnermy_FstBoss::ChangeBattleMode(bool bFlag)
{

}

void AARPGEnermy_FstBoss::HitEnd()
{
}

void AARPGEnermy_FstBoss::Attack(int32 index)
{
}

void AARPGEnermy_FstBoss::Guard(bool bFlag)
{
}

void AARPGEnermy_FstBoss::Parring(bool bFlag)
{
}

void AARPGEnermy_FstBoss::Death()
{

}

void AARPGEnermy_FstBoss::ParringHit()
{

}

void AARPGEnermy_FstBoss::DeathCollsionEnabled()
{

}

void AARPGEnermy_FstBoss::DeathWeaponSimulate()
{

}


//보스의 경우 ZeroAP는 다르게 사용
void AARPGEnermy_FstBoss::ZeroAP()
{
	Super::ZeroAP();
}
