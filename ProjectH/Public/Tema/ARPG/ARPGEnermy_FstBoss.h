// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "ARPGEnermy_FstBoss.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AARPGEnermy_FstBoss : public AARPGEnermy
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AARPGEnermy_FstBoss();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;
	//virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void TakeDamageAP(float Damage) override;
	virtual void Hit() override;
	//공격 당했는지 판단 하는 함수.	
	virtual bool CanThisDamage() override;
	// 공격 할수 있는지 판단 하는 함수.
	virtual void ChangeBattleMode(bool bFlag) override;
	virtual void HitEnd() override;

	virtual void Attack(int32 index) override; // 공격 함수
	virtual void Guard(bool bFlag) override;
	virtual void Parring(bool bFlag) override;
	virtual void Death() override; 	// 죽음 함수
	virtual void ParringHit() override;

	//적마다 콜리전이 다를 수도 있으므로 죽었을때 콜리전 해제하는 가상 함수.
	virtual void DeathCollsionEnabled() override;
	virtual void DeathWeaponSimulate() override;

	virtual void ZeroAP() override; // AP가 제로이다

public:
	UPROPERTY(BlueprintReadOnly)
		class UARPG_FstBossAnimInstance* FstBossAnimInstance;
	
};
