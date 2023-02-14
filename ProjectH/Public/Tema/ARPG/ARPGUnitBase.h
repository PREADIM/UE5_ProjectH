// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Tema/ARPG/UnitState.h"
#include "PhysicalSoundStruct.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "ARPGUnitBase.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnUseAP)
DECLARE_MULTICAST_DELEGATE(FOnUsingAP)
DECLARE_MULTICAST_DELEGATE(FOnEndAP)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamage, float)
DECLARE_MULTICAST_DELEGATE(FOnAttackAP)
DECLARE_MULTICAST_DELEGATE(FOnChargeAttackInit)

UCLASS()
class PROJECTH_API AARPGUnitBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARPGUnitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:
	/*----------------------------

			virtual function

	----------------------------*/
	virtual void Tick(float DeltaSeconds);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	virtual void TakeDamageAP(float Damage) {}

	// �ش� �Լ��� ��� Ŭ�������� ���� �����ؾ��Ѵ�.
	// �¾����� �� �Լ��� ���������� �����ϰ� �� ������ ��庰�� �˾Ƽ� �ִ��ν��Ͻ��� �ش��ϴ� ��� ��Ʈ��
	// �˷��ִ� ����
	virtual bool Hit(bool bBlockingHit);

	// �и��� ����.
	virtual void ParringHit(class AARPGUnitBase* InstigatorActor) {}
	virtual void ZeroAP(); // ������ ����ؼ� AP�� �����̴�
	
	virtual void DeathWeaponSimulate() {}
	virtual void DeathCollsionEnabled() {}
	// ����
	virtual void Death();
	//������ �������� �ݵ�� ȣ��Ǿ���� �Լ����� ��Ƶ� ��.
	virtual void EndAttack() {}


public:
	// TakeDamage ��� �� �Լ����� ������ ó���� �˾Ƽ� �ϰ��Ѵ�.
	virtual float TakeDamageCalculator(class AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser);
	float CalculDamage(float Damage);
	float CalculAPDamage(float APDamage);
	bool CanUseAP(); // AP�� �����ֳ�?

	// �и����Ҽ� �ִ��� �Ǵ��ϴ� �Լ�. (�������� ���� ��� �Լ� �ʿ�).
	bool ParringHitFunc(FVector TargetLocation);
	//TargetCharacter�� this ĳ������ ���� ����. ��ġ ����, ���ϴ� ������ �����ؼ� bool�� �޾ƿ�.
	bool TargetDotProduct(FVector TargetLocation, float CompareCos);


	void BattleHPWidgetHide();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UProjectHGameInstance* GI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGGameMode* GM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
		class UWidgetComponent* BattleHPComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UARPGWidget_BattleHP* BattleHP;



	/* ������ ���͸��� ���� */

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EPhysicalSurface>, FPhysicalSoundStruct> PhysicalAllSounds;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class USoundBase*> PhysicalSounds;*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FPhysicalSoundStruct PhysicalSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SurfaceDistance; // ������ �Ÿ�

	// ���带 �ش� ǥ�鿡 ���缭 �����صΰ� �ִԺ������� �÷��� �ϵ��� �Ѵ�.
	void SetPhysicalSound();


	//-----------���� �и�---------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FUnitState UnitState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentAttackFactor; // ���� ������ ���

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentAP_DMG; // �߰� AP ������

	//-------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bSpecialAttackMode; // �ش� ������ true�� �����̿�����.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bSpecialAttackPlaying; // �ش� ������ ����� ������ ����� ������ �ٽ� �ѹ� �ϴ� ���� ���� �ϱ� ����.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDontMoving; // �������̰ų� ��Ʈ���϶� �����̴� �� ����.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDontParringAttack; // �и��� �ȵǴ� �������ΰ�? (�и� �Ұ� ����)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParring;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParringHit; // �и��� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDeath;

	//�и� ������ �� ���ִ��� ����. �ҿ��ó�� Ư���� ��ȣ�ۿ� �и��� �Ϸ��� ���� �����Ҷ�
	//������ ��� ���� �и��Ǿ��ٴ� ������ True�ϰ� �� ĳ���Ϳ��� �ڽ��� ������ ������ �Ѱ��ָ� �ɵ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bCanParringAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bHitting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BattleSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BackSpeed;

	//------------------------------------------
	// Use AP ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlockingDEF; // �ش� ������ ���ŷ �Ҽ� �ִ� ���. (���д� 100, ������ 80���� ����.)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float APSpeed; // AP�� ���� �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float APUseSpeed; // AP�� ���Ǵ� �ӵ�

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bUseAP; // �����̳� ����� ��� �Ҷ� ��Ÿ�ְ� ������ AP�� ������ ����.
	// ��Ÿ�� ���� -> true ��Ÿ�� ������ End

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bUsingAP; // �����ؼ� ��� ����ϴ� ��� ex) �޸���, ��¡ ���� 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bZeroAP; // AP�� ���θ� ������쿣 ������ ���¹̳��� �ٷ������ʰ� ���� ������.
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttackAndHitAP; // �����̳� ����� ������ ��� 1�� ��ٷȴٰ� AP�� �����Ѵ�.

	const float ZeroAPWaitTime = 1.5f; // 1.5�ʰ� ���
	const float AttackAPWaitTime = 1.0f; // 1�ʰ� ���
	float CurrentWaitTime = 0.0f;

	//-------------------------------------------

	FOnUseAP OnUseAP; // AP�� ����� �� ȣ���� ��������Ʈ
	FOnUsingAP OnUsingAP; // AP�� ���������� ����� �� ����� ��������Ʈ
	FOnUseAP OnEndAP; // ��Ÿ�ְ� ���� AP�� �ٽ� ������ �� ����� ��������Ʈ
	FOnDamage OnDamage; // ������ ó���� �Ҷ� ȣ��� �Լ�. ���ڷ� float
	FOnAttackAP OnAttackAP; // ���ݽó� ���� �������� AP ���� ��������Ʈ.
	FOnChargeAttackInit OnChargeAttackInit; // �������� ���۽� ��������Ʈ
};
