// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Tema/ARPG/UnitState.h"
#include "PhysicalSoundStruct.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "ARPGUnitBase.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnUseAP);
DECLARE_MULTICAST_DELEGATE(FOnUsingAP);
DECLARE_MULTICAST_DELEGATE(FOnEndAP);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamage, float);
DECLARE_MULTICAST_DELEGATE(FOnAttackAP);
DECLARE_MULTICAST_DELEGATE(FOnChargeAttackInit);

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
	/*------------------------
		virtual function
	--------------------------*/
	virtual void Tick(float DeltaSeconds);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	virtual void TakeDamageAP(float Damage) {}

	// 해당 함수는 모든 클래스들이 따로 제작해야한다.
	// 맞았을때 이 함수를 공통적으로 실행하고 각 유닛의 모드별로 알아서 애님인스턴스에 해당하는 모드 히트를
	// 알려주는 형식
	virtual bool Hit(bool bBlockingHit);

	// 패링에 당함.
	virtual void ParringHit(class AARPGUnitBase* InstigatorActor) {}
	virtual void ZeroAP(); // 공격을 사용해서 AP가 제로이다
	
	virtual void DeathWeaponSimulate() {}
	virtual void DeathCollsionEnabled() {}
	// 죽음
	virtual void Death();
	//공격이 중단되거나 끝났을때 반드시 호출되어야할 함수들을 모아둔 것.
	virtual void EndAttack() {}
	virtual void SpecialAttackHitMontage() {} // 스페셜 어택을 당했을때 해당몽타주를 실행한다.

public:
	// TakeDamage 대신 이 함수에서 데미지 처리를 알아서 하게한다.
	virtual float TakeDamageCalculator(class AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser);
	virtual float DamageFunction(class AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser);
	// 같은 함수가 이름만 다르게 두개 있는 이유는 데미지 처리는 TakeDamage에서 담당하지만, 특수한 경우 (패링 공격) 등은 
	// 무조건 데미지가 들어가야하므로 DamageFunction을 호출.

	float CalculDamage(float Damage);
	float CalculAPDamage(float APDamage);
	bool CanUseAP(); // AP가 쓸수있나?


	// 패링당할수 있는지 판단하는 함수. (내적으로 각도 계산 함수 필요).
	bool ParringHitFunc(FVector TargetLocation);
	//TargetCharacter와 this 캐릭터의 내적 연산. 위치 값과, 원하는 각도를 대입해서 bool로 받아옴.
	bool TargetDotProduct(FVector TargetLocation, float CompareCos);

	UFUNCTION()
		void BattleHPWidgetHide();
public:
	/*----------------
		공통 변수
	------------------*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UProjectHGameInstance* GI;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGGameMode* GM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
		class UWidgetComponent* BattleHPComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UARPGWidget_BattleHP* BattleHP;
	UPROPERTY(VisibleAnywhere)
		float MouseSensitivity;

	/*-------------------------
		피지컬 머터리얼 사운드
	---------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EPhysicalSurface>, FPhysicalSoundStruct> PhysicalAllSounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FPhysicalSoundStruct PhysicalSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SurfaceDistance; // 땅끝의 거리

	// 사운드를 해당 표면에 맞춰서 설정해두고 애님블프에서 플레이 하도록 한다.
	void SetPhysicalSound();

	/*-----------------------------------------------------
		죽고 무언가 실행해야할 때 BP에서 따로 바인드하는 함수
	-------------------------------------------------------*/

	UFUNCTION(BlueprintImplementableEvent)
		void BPBindDeath();

	/*-----------------
		공통 스탯 관련
	-------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FUnitState UnitState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentAttackFactor; // 현재 공격의 계수

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentAP_DMG; // 추가 AP 데미지


	/*---------------
		상태 변수
	-----------------*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bSpecialAttackMode; // 해당 변수가 true면 무적이여야함.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bSpecialAttackPlaying; // 해당 변수는 스페셜 어택중 스페셜 어택을 다시 한번 하는 것을 방지 하기 위함.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDontMoving; // 공격중이거나 히트중일때 움직이는 것 방지.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDontParringAttack; // 패링이 안되는 공격중인가? (패링 불가 어택)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParring;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParringHit; // 패링에 당함

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bSpecialMontageHitting = false; // 특수 공격의 몽타주가 실행중이다.

	UPROPERTY(VisibleAnywhere)
		class AARPGUnitBase* CanSATargetUnit; // 스페셜 어택을 해당 유닛에게 가능하다.


	//패링 어택을 할 수있는지 여부. 소울류처럼 특별한 상호작용 패링을 하려면 적이 공격할때
	//나에게 닿는 순간 패링되었다는 변수를 True하고 내 캐릭터에게 자신의 포인터 정보를 넘겨주면 될듯.
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
	
	/*----------------
		 AP 관련
	------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlockingDEF; // 해당 무기의 블록킹 할수 있는 계수. (방패는 100, 어떤무기는 80같은 느낌.)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float APSpeed; // AP가 차는 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float APUseSpeed; // AP가 사용되는 속도

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bUseAP; // 공격이나 기술을 사용 할때 몽타주가 끝나야 AP가 오르는 형식.
	// 몽타주 실행 -> true 몽타주 끝나면 End

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bUsingAP; // 지속해서 계속 사용하는 경우 ex) 달리기, 차징 공격 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bZeroAP; // AP가 제로를 찍은경우엔 무조건 스태미나가 바로차지않고 텀을 가진다.
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttackAndHitAP; // 공격이나 쉴드로 막았을 경우 1초 기다렸다가 AP가 차게한다.

	const float ZeroAPWaitTime = 1.5f; // 1.5초간 대기
	const float AttackAPWaitTime = 1.0f; // 1초간 대기
	float CurrentWaitTime = 0.0f;

	/*---------------
		델리게이트
	------------------*/

	FOnUseAP OnUseAP; // AP를 사용할 때 호출할 델리게이트
	FOnUsingAP OnUsingAP; // AP를 지속적으로 사용할 때 사용할 델리게이트
	FOnUseAP OnEndAP; // 몽타주가 끝나 AP가 다시 차야할 때 사용할 델리게이트
	FOnDamage OnDamage; // 데미지 처리를 할때 호출될 함수. 인자로 float
	FOnAttackAP OnAttackAP; // 공격시나 공격 당했을시 AP 차감 델리게이트.
	FOnChargeAttackInit OnChargeAttackInit; // 차지어택 시작시 델리게이트
};
