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
	//virtual float TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeDamageCalculator(class AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser) override;
	virtual void TakeDamageAP(float Damage) override;
	virtual bool Hit(bool bBlockingHit) override;
	virtual void SetBattleMode(bool bFlag) override;

	virtual void HitEnd() override;

	virtual void Attack(int32 index) override; // 공격 함수
	virtual void Death() override; 	// 죽음 함수
	virtual void ParringHit(class AARPGUnitBase* InstigatorActor) override;

	//적마다 콜리전이 다를 수도 있으므로 죽었을때 콜리전 해제하는 가상 함수.
	virtual void DeathCollsionEnabled() override;
	virtual void DeathWeaponSimulate() override;

	virtual void ZeroAP() override; // AP가 제로이다

public:
	void DeathReset(); // 죽어서 불필요한 변수들 초기화,

	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	UFUNCTION(BlueprintCallable)
		void WeaponOverlapEnd(int32 Num); // 0양손, 1왼손, 2오른손, 3모두

	UFUNCTION(BlueprintCallable)
		void SetWeaponCollision(bool bFlag, int32 index); // 0양손, 1왼손, 2오른손

	UFUNCTION(BlueprintCallable)
		void SetWeaponEndCollision(); // 양손 무기 콜리전 둘다 꺼버리기

	void PlayAttack(int32 index); // AttackComponent안에 있는 TArray형의 어택들에 인덱스를 보내
	// 해당 인덱스의 어택이 발동되게하는 방식.


public:
	UPROPERTY(BlueprintReadOnly)
		class UARPG_FstBossAnimInstance* FstBossAnimInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_LeftWeapon;
	UPROPERTY()
		class AARPGWeapon* LeftWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_RightWeapon;
	UPROPERTY()
		class AARPGWeapon* RightWeapon;

	// 두손 한번에 공격했을때 데미지를 한번만 적용하기위한 투명 콜리전
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_TwinWeapon;
	UPROPERTY()
		class AARPGWeapon* TwinWeapon;

	/*------------------------------------------------*/

	// 스킬에 필요한 소환체들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGFstBoss_Projectile> BP_ComboProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGFstBoss_Projectile> BP_ComboProjectile_Twin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGFstBoss_Projectile> BP_R_Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGFstBoss_Projectile> BP_E_Projectile;

};
