// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "PhysicalSoundStruct.h"
#include "ARPGWeapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponSFX : uint8
{
	Default UMETA(DisplayName = "Default"),
	DropSFX UMETA(DisplayName = "DropSFX"),
	PhysicsOverlapSFX UMETA(DisplayName = "PhysicsOverlapSFX"),
	SwingSFX UMETA(DisplayName = "SwingSFX"),
	ChargeSwingSFX UMETA(DisplayName = "ChargeSwingSFX"),
	AttackHitSFX UMETA(DisplayName = "AttackHitSFX"),
	ParringHitSFX UMETA(DisplayName = "ParringHitSFX"), // �и��������� ���⿡�� �� �Ҹ�
	SpecialAttackSFX UMETA(DisplayName = "SpecialAttackSFX")
};


UCLASS()
class PROJECTH_API AARPGWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARPGWeapon();
	class AController* GetOwnerController();

public:
	virtual void AttackEnd() {}
	virtual void SetWeaponCollision(bool bFlag) {}
	virtual void SetPhysics();
	virtual void SetOwnerNoSee(bool bFlag) {}
	virtual void SetWeaponNoSee() {} /* �ƿ� ������ �ʰ� �ϱ� */

	UFUNCTION()
		void DestroyWeapon();
	
	// ���� ������ �������� ���� Ȯ���ϰ� true�� float������ ���� �󸶳� �����Ǿ����� �˷��ش�.
	virtual bool IsChargeAttack() { return false; } // ���⸶�� ���������� ���� ���� ���� ���� �ִ�.
	UFUNCTION()
		virtual void ChargeAttackInit();

	virtual float ChargeAttack(float DeltaSeconds) { return 0.f; }

	// ������ ����Ǿ����� ���⸶�� �ʱ�ȭ �ؾ��ϴ� ��ġ�� �ִٸ� �� �Լ��� �̿�.
	virtual void WeaponAttackEnd() {}


	virtual void PlayWeaponSound(EWeaponSFX WeaponSFX);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

	UPROPERTY(VisibleAnywhere)
		TArray<AActor*> HitEndActor;

	UFUNCTION()
		void SetHitEndActor() { HitEndActor.Empty(); } // �����͵� �ʱ�ȭ.
public:
	UPROPERTY()
		class AARPGUnitBase* OwnerUnit;

	// ���⸶�� AP�� ��� ��ġ�� �ٸ� ���� �����Ƿ� �̰����� ���
	// ���ݸ����� AP�� ��� ��ġ�� �� �Ѵ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponAP_DMG = 0.0f;
	
	// ���⸶�� �ش� ���⸦ �ֵθ��� �ִ� �ʿ� AP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UseAP;

	// �ش� ���Ⱑ ������ ���´ٸ� ������ ��ġ. (���д� 100)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlockingDEF;

	//----------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDamage")
		float WeaponDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ChargeAttack")
		float Charge = 1.f; // ���� ������ ���

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ChargeAttack")
		float ChargeRatio; // ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChargeAttack")
		float MaxChargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChargeAttack")
		float ChargeTime;

	// ���� ���ε�. �����ϸ� ������� ���ε��� �ø��� ����. ���� �׳� �� ��ü���� ���Ѵ�.
	// �� ������ ��� ���� ��ü�� ������ ���� ���� �߰��� ���� �ϴ� ���Ļ� ����� �δ� ��.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poise")
		float WeaponPoise;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		TMap<EWeaponSFX, FSoundAndAttenuation> WeaponSFXs;
	

	//-------------------------------------------
	ECollisionChannel ARPGUnitChannel;
};
