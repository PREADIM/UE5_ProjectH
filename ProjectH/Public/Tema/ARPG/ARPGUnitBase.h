// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Tema/ARPG/UnitState.h"
#include "ARPGUnitBase.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnUseAP)
DECLARE_MULTICAST_DELEGATE(FOnUsingAP)
DECLARE_MULTICAST_DELEGATE(FOnEndAP)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamage, float)

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

	// 해당 함수는 모든 클래스들이 따로 제작해야한다.
	// 맞았을때 이 함수를 공통적으로 실행하고 각 유닛의 모드별로 알아서 애님인스턴스에 해당하는 모드 히트를
	// 알려주는 형식
	virtual bool Hit(bool bBlockingHit);

	// 패링에 당함.
	virtual void ParringHit(class AARPGUnitBase* InstigatorActor) {}
	virtual void ZeroAP(); // 공격을 사용해서 AP가 제로이다
	
	virtual void DeathWeaponSimulate() {}
	// 죽음
	virtual void Death();
	//공격이 끝났을때 반드시 호출되어야할 함수들을 모아둔 것.
	virtual void EndAttack() {}
public:
	// TakeDamage 대신 이 함수에서 데미지 처리를 알아서 하게한다.
	//virtual float TakeDamageCalculator (float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	virtual float TakeDamageCalculator(class AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser);
	float CalculDamage(float Damage);
	float CalculAPDamage(float APDamage);
	bool CanUseAP(); // AP가 쓸수있나?

	// 패링당할수 있는지 판단하는 함수. (내적으로 각도 계산 함수 필요).
	bool ParringHitFunc(FVector TargetLocation);
	//TargetCharacter와 this 캐릭터의 내적 연산. 위치 값과, 원하는 각도를 대입해서 bool로 받아옴.
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

	//-----------공통 분모---------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FUnitState UnitState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentAttackFactor; // 현재 공격의 계수

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentAP_DMG; // 추가 AP 데미지

	//-------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bSpecialAttackMode; // 해당 변수가 true면 무적이여야함.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bSpecialAttackPlaying; // 해당 변수가 true면 움직이면 안된다.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDontParringAttack; // 패링이 안되는 공격중인가? (패링 불가 어택)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParring;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParringHit; // 패링에 당함

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDeath;

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


	//------------------------------------------
	// Use AP 관련

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

	const float ZeroAPWaitTime = 1.0f; // 1초간 대기
	float CurrentWaitTime = 0.0f;

	//-------------------------------------------

	FOnUseAP OnUseAP; // AP를 사용할 때 호출할 델리게이트
	FOnUsingAP OnUsingAP; // AP를 지속적으로 사용할 때 사용할 델리게이트
	FOnUseAP OnEndAP; // 몽타주가 끝나 AP가 다시 차야할 때 사용할 델리게이트
	FOnDamage OnDamage; // 데미지 처리를 할때 호출될 함수. 인자로 float


};
