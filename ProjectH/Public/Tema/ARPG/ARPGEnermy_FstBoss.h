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
	//���� ���ߴ��� �Ǵ� �ϴ� �Լ�.	
	virtual bool CanThisDamage() override;
	// ���� �Ҽ� �ִ��� �Ǵ� �ϴ� �Լ�.
	virtual void ChangeBattleMode(bool bFlag) override;
	virtual void HitEnd() override;

	virtual void Attack(int32 index) override; // ���� �Լ�
	virtual void Guard(bool bFlag) override;
	virtual void Parring(bool bFlag) override;
	virtual void Death() override; 	// ���� �Լ�
	virtual void ParringHit() override;

	//������ �ݸ����� �ٸ� ���� �����Ƿ� �׾����� �ݸ��� �����ϴ� ���� �Լ�.
	virtual void DeathCollsionEnabled() override;
	virtual void DeathWeaponSimulate() override;

	virtual void ZeroAP() override; // AP�� �����̴�

public:
	UPROPERTY(BlueprintReadOnly)
		class UARPG_FstBossAnimInstance* FstBossAnimInstance;
	
};
