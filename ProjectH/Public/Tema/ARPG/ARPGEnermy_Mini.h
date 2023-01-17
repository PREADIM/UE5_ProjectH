// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "ARPGEnermy_Mini.generated.h"


// ����� ���и� ����ִ� ���� ���� Ŭ����.
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
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
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



	//---------------------------------------

	void DeathReset(); // �׾ ���ʿ��� ������ �ʱ�ȭ,

	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	UFUNCTION(BlueprintCallable)
		void WeaponOverlapEnd();

	void PlayAttack(int32 index); // AttackComponent�ȿ� �ִ� TArray���� ���õ鿡 �ε����� ����
	// �ش� �ε����� ������ �ߵ��ǰ��ϴ� ���.

	UFUNCTION(BlueprintCallable)
		void SetWeaponCollision(bool bFlag);

	UFUNCTION(BlueprintCallable)
		void SetShieldCollision(bool bFlag);

	//-----------------------------------------

	void SetBlocking(bool bFlag); // BT���� ���и� ����� ��������

public:
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
