// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/ARPGUnitBase.h"
#include "ARPGEnermy.generated.h"


// ��� ���� ���� �θ� ��� ���� ������ �־���� �͵鸸 �������־����.
// (ex. ���� ����, �ӵ�, ������, �и��� , ���ŷ �� ���� ���� �͵�)


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack)
DECLARE_MULTICAST_DELEGATE(FOnMoving)

UENUM(BlueprintType)
enum class EEnermyType : uint8
{
	Unit UMETA(DisplayName = "Unit"),
	Boss UMETA(DisplayName = "Boss")
};


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

// ���� ��� �����ϰ��� �ش� Enum ������ MovingState�� �����Ѵ�.

UCLASS()
class PROJECTH_API AARPGEnermy : public AARPGUnitBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARPGEnermy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/*----------------------------

			virtual function

	----------------------------*/


	virtual void Tick(float DeltaSeconds);
	virtual void PostInitializeComponents();
	//virtual float TakeDamageCalculator(float APDamage, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	virtual float TakeDamageCalculator(class AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser);
	virtual void TakeDamageAP(float Damage) {}
	// �¾����� �Լ�. UnitBase���� ���� ���ε��� ������ ��Ʈ ����� ���ð����� �Ǻ��Ѵ�.
	virtual bool Hit(bool bBlockingHit) { return Super::Hit(bBlocking); } 
	virtual bool CanThisDamage() { return false; }
	virtual void HitEnd() {}
	virtual void SetBattleMode(bool bFlag);


	virtual void Attack(int32 index) {} // ���� �Լ�
	virtual void Guard(bool bFlag) {} // ���� �Լ�
	virtual void Parring(bool bFlag) {} // �и� �Լ�
	virtual void Death(); // ���� �Լ�
	virtual void ParringHit(class AARPGUnitBase* InstigatorActor) {} // �и�����
	virtual void DeathCollsionEnabled() {} 
	virtual void DeathWeaponSimulate() {} // �׾ ���� ���� �ùķ���Ʈ
	virtual void ZeroAP(); // AP�� �����̴�


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackComponent", meta = (AllowPrivateAccess = "true"))
		class UARPGAttackComponent* AttackComponent;
	class UARPGAttackComponent* GetAttackComponent() { return AttackComponent; }

public:
	UPROPERTY(BlueprintAssignable)
		FOnAttack OnAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBattleMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bMoving; // �� ������ �����̰ų� �齺���� ��.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDontLockOn; // �÷��̾�� ȭ�� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackCoolTime; // ���� ������

	const float& GetAttackCoolTime() { return AttackCoolTime; }

	//-----------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEnermyType EnermyType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EMovingState MainMovingState; // ���� �Է� �Ǵ���.

	// ���� ��� �������� �ش� ������ EnermyMoveState�� �����Ѵ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EEnermyMoveMode State = EEnermyMoveMode::None;

	//-----------------------
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentCollisionRadius; // ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalCollisionRadius; // ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BattleCollisionRadius; // ��Ʋ ���� ����

	float GetCollisionRadius() { return CurrentCollisionRadius; }
	void SetCollisionRadius(bool bFlag); // �÷��̾ ã������ ���������� �þ��, �ƴѰ��� ���� �������� (���� �������)

	//----------------------------

	// �� ������ ���� �¼��� ���� ����. ������ �ٸ��⶧���� �� ���� �ǽð����� �����ͼ� �ٲ۴�.
	// ������ ��쿡�� ������ ���� -> ���� �̹Ƿ� �ʿ����� �������� ����.
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BattleDistance;*/

	float GetBattleDistance();
	//void SetBattleDistance(float Distance) { BattleDistance = Distance; }


	//--------------------------------


	UPROPERTY()
		class AARPGAIController* OwnerAIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnit* PlayerUnit; // BT���� �������Ͽ� ������ Ÿ��.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BT;
	class UBehaviorTree* GetBT() { return BT; }

	//------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class UAttackClass>> BP_Attacks;
	// �ڡ� ���� ������Ʈ���� ����� ���� Ŭ������

	//------------------------------------


public:
	UFUNCTION(BlueprintImplementableEvent)
		void EnermyMoving(); // �������Ʈ���� �� ���� �˸´� �ൿ �ϰ��ϱ� �������� Tick���� ������ ���� �Լ�.

	UFUNCTION(BlueprintCallable)
		FVector GetMovingValue(); // �����̴��� Value�� ���س���

	UFUNCTION(BlueprintCallable)
		void LockOnPlayer(float DeltaSeconds); // SetActorRotation���� �÷��̾� �����ϴ� �Լ�.

	UFUNCTION(BlueprintCallable)
		void SetState();

	UFUNCTION(BlueprintCallable)
		float GetDirection(FVector MoveDirection); // ���� � �������� �ִԺ����� �����ϴ� �Լ�.

	void SetEnermyMoveMode(EEnermyMoveMode Mode); // ������ ��� ����


	//--------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlockMode; // �̰� �ִϸ��̼��� ���� ����.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlocking; // �̰� ���� �� �ܰ����� �˻��ϴ� ����. �̰��� true���� ���� ���� �Ϸ� �ΰ�.
};
