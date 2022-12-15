// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "ARPGEnermy.generated.h"


// 모든 적의 공통 부모 모든 적이 가지고 있어야할 것들만 가지고있어야함.
// (ex. 수색 범위, 속도, 공격중, 패링중 , 블록킹 등 상태 같은 것들)

DECLARE_MULTICAST_DELEGATE(FOnAttack)
DECLARE_MULTICAST_DELEGATE(FOnMoving)

UENUM(BlueprintType)
enum class EMovingState : uint8
{
	Stop UMETA(DisplayName = "Stop"),
	Forward UMETA(DisplayName = "Forward"),
	Back UMETA(DisplayName = "Back"),
	Right UMETA(DisplayName = "Right"),
	Left UMETA(DisplayName = "Left")
};

UENUM(BlueprintType)
enum class EEnermyMoveMode : uint8
{
	None UMETA(DisplayName = "None"),
	LeftMoving UMETA(DisplayName = "LeftMoving"),
	RightMoving UMETA(DisplayName = "RightMoving"),
	ForwardMoving UMETA(DisplayName = "ForwardMoving"),
	BackMoving UMETA(DisplayName = "BackMoving")
};

// 적이 어떻게 움직일건지 해당 Enum 값으로 MovingState를 결정한다.

UCLASS()
class PROJECTH_API AARPGEnermy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARPGEnermy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents();

public:
	FOnAttack OnAttack;
	FOnMoving OnMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParring;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBattleMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bMoving; // 양 옆으로 움직이거나 백스텝을 함.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BattleSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PrevSpeed; // 백무빙을 할때는 속도가 줄어야 하므로 잠시 저장용


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EMovingState EnermyMoveState; // 어디로 입력 되는지.

	// 적이 어떻게 움직일지 해당 값으로 EnermyMoveState를 결정한다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EEnermyMoveMode State = EEnermyMoveMode::None;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentCollisionRadius; // 현재 수색 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalCollisionRadius; // 평상시 수색 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BattleCollisionRadius; // 배틀 수색 범위

	// 이 유닛의 전투 태세에 들어가는 범위. 적마다 다르기때문에 이 값을 실시간으로 가져와서 바꾼다.
	// 보스의 경우에는 무조건 락온 -> 공격 이므로 필요하지 않을수도 있음.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float BattleDistance; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalDistance;

	// 공격 거리를 나타내는 변수 적마다 다르기때문에 따로 이 값을 수시로 바꿔준다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackDistance;

	float GetAttackDistance() { return AttackDistance; }


	UPROPERTY()
		class AARPGAIController* OwnerAIController;

	UPROPERTY(BlueprintReadOnly)
		class UARPG_EnermyAnimInstance* EnermyAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnit* PlayerUnit; // BT에서 오버랩하여 가져온 타겟.


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BT;
	class UBehaviorTree* GetBT() { return BT; }

public:
	virtual void Attack() {} // 공격 함수
	virtual void Garud() {} // 막는 함수


	UFUNCTION(BlueprintImplementableEvent)
		void EnermyMoving(); // 블루프린트에서 각 적에 알맞는 행동 하게하기 실질적인 Tick에서 실행할 메인 함수.

	UFUNCTION(BlueprintCallable)
		FVector GetMovingValue(); // 움직이는지 Value값 구해내기

	UFUNCTION(BlueprintCallable)
		void LockOnPlayer(); // SetActorRotation으로 플레이어 락온하는 함수.

	UFUNCTION(BlueprintImplementableEvent)
		void ParringMontage();

	UFUNCTION(BlueprintCallable)
		float GetDirection(FVector MoveDirection); // 현재 어떤 방향인지 애님블프에 전달하는 함수.


	float GetCollisionRadius() { return CurrentCollisionRadius; }
	void SetCollisionRadius(bool bFlag); // 플레이어를 찾았으면 수색범위가 늘어나고, 아닌경우는 원래 수색범위 (원신 같은경우)
	

	float GetBattleDistance() { return BattleDistance; }
	void SetBattleDistance(float Distance) { BattleDistance = Distance; }

	void SetBlocking(bool bFlag); // BT에서 방패를 들건지 설정해줌
	void SetBattleMode(bool bFlag); // BT에서 방패를 들건지 설정해줌

	void SetEnermyMoveMode(EEnermyMoveMode Mode); // 움직일 모드 설정

	void CastMoving();
	//무빙이 끝나고 캐스트하여 무빙이 다 끝났다는걸 알려주는 함수.
};
