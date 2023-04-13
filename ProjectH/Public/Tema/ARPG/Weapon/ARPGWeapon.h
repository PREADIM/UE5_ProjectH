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
	ParringHitSFX UMETA(DisplayName = "ParringHitSFX"), // 패링당했을때 무기에서 날 소리
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
	virtual void SetWeaponNoSee() {} /* 아예 보이지 않게 하기 */

	UFUNCTION()
		void DestroyWeapon();
	
	// 차지 어택이 가능한지 먼저 확인하고 true면 float형으로 현재 얼마나 차지되었는지 알려준다.
	virtual bool IsChargeAttack() { return false; } // 무기마다 차지어택이 있을 수도 없을 수도 있다.
	UFUNCTION()
		virtual void ChargeAttackInit();

	virtual float ChargeAttack(float DeltaSeconds) { return 0.f; }

	// 공격이 종료되었을때 무기마다 초기화 해야하는 수치가 있다면 이 함수를 이용.
	virtual void WeaponAttackEnd() {}


	virtual void PlayWeaponSound(EWeaponSFX WeaponSFX);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

	UPROPERTY(VisibleAnywhere)
		TArray<AActor*> HitEndActor;

	UFUNCTION()
		void SetHitEndActor() { HitEndActor.Empty(); } // 때린것들 초기화.
public:
	UPROPERTY()
		class AARPGUnitBase* OwnerUnit;

	// 무기마다 AP를 까는 수치가 다를 수도 있으므로 이것으로 계산
	// 공격마다의 AP를 까는 수치를 더 한다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponAP_DMG = 0.0f;
	
	// 무기마다 해당 무기를 휘두를수 있는 필요 AP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UseAP;

	// 해당 무기가 공격을 막는다면 감쇄할 수치. (방패는 100)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlockingDEF;

	//----------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDamage")
		float WeaponDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ChargeAttack")
		float Charge = 1.f; // 차지 데미지 계수

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ChargeAttack")
		float ChargeRatio; // 차지 비율

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChargeAttack")
		float MaxChargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChargeAttack")
		float ChargeTime;

	// 무기 강인도. 장착하면 사용자의 강인도를 올리는 형식. 적은 그냥 적 자체에서 정한다.
	// 이 게임의 경우 무기 교체가 없지만 추후 무기 추가를 위해 일단 형식상 만들어 두는 것.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poise")
		float WeaponPoise;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		TMap<EWeaponSFX, FSoundAndAttenuation> WeaponSFXs;
	

	//-------------------------------------------
	ECollisionChannel ARPGUnitChannel;
};
