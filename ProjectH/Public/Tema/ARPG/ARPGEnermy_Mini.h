// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "ARPGEnermy_Mini.generated.h"


// 무기와 방패를 들고있는 적에 대한 클래스.
/**
 * 
 */
UCLASS()
class PROJECTH_API AARPGEnermy_Mini : public AARPGEnermy
{
	GENERATED_BODY()

public:
	AARPGEnermy_Mini();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;
	//virtual float TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeDamageCalculator(class AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void TakeDamageAP(float Damage) override;
	virtual bool Hit(bool bBlockingHit) override;
	// 공격 할수 있는지 판단 하는 함수.
	virtual void ChangeBattleMode(bool bFlag) override;
	virtual void HitEnd() override;

	virtual void Attack(int32 index) override; // 공격 함수
	virtual void Guard(bool bFlag) override;
	virtual void Parring(bool bFlag) override;
	virtual void Death() override; 	// 죽음 함수
	virtual void ParringHit(class AARPGUnitBase* InstigatorActor) override;

	//적마다 콜리전이 다를 수도 있으므로 죽었을때 콜리전 해제하는 가상 함수.
	virtual void DeathCollsionEnabled() override;
	virtual void DeathWeaponSimulate() override;

	virtual void ZeroAP() override; // AP가 제로이다


	//---------------------------------------

	void DeathReset(); // 죽어서 불필요한 변수들 초기화,

	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	UFUNCTION(BlueprintCallable)
		void WeaponOverlapEnd();

	void PlayAttack(int32 index); // AttackComponent안에 있는 TArray형의 어택들에 인덱스를 보내
	// 해당 인덱스의 어택이 발동되게하는 방식.

	UFUNCTION(BlueprintCallable)
		void SetWeaponCollision(bool bFlag);

	UFUNCTION(BlueprintCallable)
		void SetShieldCollision(bool bFlag);

	//-----------------------------------------

	void SetBlocking(bool bFlag); // BT에서 방패를 들건지 설정해줌

public:
	UPROPERTY(BlueprintReadOnly)
		class UARPG_EnermyAnimInstance* EnermyAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGWeapon* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_Shield;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGWeapon* Shield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ShieldSockName;

};
