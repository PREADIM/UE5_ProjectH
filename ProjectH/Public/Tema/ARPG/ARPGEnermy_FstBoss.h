// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "ARPGEnermy_FstBoss.generated.h"

/**
 * 
 */

// �и��� �ȵǴ� ���۾Ƹ� �����϶� ���¸� �ľ��ϱ� ���� ������ Ŭ����
// �ַ� AttackClass�� ���ε� �Լ����� �����ҵ�.
UENUM(BlueprintType)
enum class ESuperAttackingState : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	DiveAttacking UMETA(DiplayName = "DiveAttacking")
};


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
	virtual float TakeDamageCalculator(class AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void TakeDamageAP(float Damage) override;
	virtual bool Hit(bool bBlockingHit) override;

	// ���� �Ҽ� �ִ��� �Ǵ� �ϴ� �Լ�.
	virtual void ChangeBattleMode(bool bFlag) override;
	virtual void HitEnd() override;

	virtual void Attack(int32 index) override; // ���� �Լ�
	virtual void Death() override; 	// ���� �Լ�
	virtual void ParringHit(class AARPGUnitBase* InstigatorActor) override;

	//������ �ݸ����� �ٸ� ���� �����Ƿ� �׾����� �ݸ��� �����ϴ� ���� �Լ�.
	virtual void DeathCollsionEnabled() override;
	virtual void DeathWeaponSimulate() override;

	virtual void ZeroAP() override; // AP�� �����̴�

public:
	void DeathReset(); // �׾ ���ʿ��� ������ �ʱ�ȭ,

	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	UFUNCTION(BlueprintCallable)
		void WeaponOverlapEnd();

	UFUNCTION(BlueprintCallable)
		void SetWeaponCollision(bool bFlag, int32 index); // 0���, 1�޼�, 2������

	UFUNCTION(BlueprintCallable)
		void SetWeaponEndCollision(); // ��� ���� �ݸ��� �Ѵ� ��������

	void PlayAttack(int32 index); // AttackComponent�ȿ� �ִ� TArray���� ���õ鿡 �ε����� ����
	// �ش� �ε����� ������ �ߵ��ǰ��ϴ� ���.


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

	// �μ� �ѹ��� ���������� �������� �ѹ��� �����ϱ����� ���� �ݸ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_TwinWeapon;
	UPROPERTY()
		class AARPGWeapon* TwinWeapon;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGFstBoss_Projectile> BP_ComboProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGFstBoss_Projectile> BP_ComboProjectile_Twin;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ESuperAttackingState SuperAttackingState = ESuperAttackingState::NONE;
};
