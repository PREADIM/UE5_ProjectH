// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/ARPGUnitBase.h"
#include "ARPGEnermy.generated.h"


// ��� ���� ���� �θ� ��� ���� ������ �־���� �͵鸸 �������־����.
// (ex. ���� ����, �ӵ�, ������, �и��� , ���ŷ �� ���� ���� �͵�)


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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual void TakeDamageAP(float Damage) {}
	virtual void Hit() {} // �¾����� �Լ�
	virtual bool CanThisDamage() { return false; }
	virtual void ChangeBattleMode(bool bFlag) {}
	virtual void HitEnd() {}

	virtual void Attack(int32 index) {} // ���� �Լ�
	virtual void Guard(bool bFlag) {} // ���� �Լ�
	virtual void Parring(bool bFlag) {} // �и� �Լ�
	virtual void Death(); // ���� �Լ�
	virtual void ParringHit() {} // �и�����
	virtual void DeathCollsionEnabled() {} 
	virtual void DeathWeaponSimulate() {} // �׾ ���� ���� �ùķ���Ʈ
	virtual void ZeroAP(); // AP�� �����̴�



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackComponent", meta = (AllowPrivateAccess = "true"))
		class UARPGAttackComponent* AttackComponent;
	class UARPGAttackComponent* GetAttackComponent() { return AttackComponent; }

public:
	FOnAttack OnAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBattleMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bMoving; // �� ������ �����̰ų� �齺���� ��.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackCoolTime; // ���� ������

	const float& GetAttackCoolTime() { return AttackCoolTime; }

	//-----------------------------------


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EMovingState EnermyMoveState; // ���� �Է� �Ǵ���.

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BattleDistance; 

	float GetBattleDistance() { return BattleDistance; }
	void SetBattleDistance(float Distance) { BattleDistance = Distance; }


	//------------------------------

	// ���� �Ÿ��� ��Ÿ���� ���� ������ �ٸ��⶧���� ���� �� ���� ���÷� �ٲ��ش�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackDistance;

	float GetAttackDistance() { return AttackDistance; }

	//--------------------------------


	UPROPERTY()
		class AARPGAIController* OwnerAIController;

	UPROPERTY(BlueprintReadOnly)
		class UARPG_EnermyAnimInstance* EnermyAnimInstance;

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
		void LockOnPlayer(); // SetActorRotation���� �÷��̾� �����ϴ� �Լ�.

	UFUNCTION(BlueprintCallable)
		float GetDirection(FVector MoveDirection); // ���� � �������� �ִԺ����� �����ϴ� �Լ�.

	void SetEnermyMoveMode(EEnermyMoveMode Mode); // ������ ��� ����

	//-----------------------------------------

	void SetBattleMode(bool bFlag); 

	//-----------------------------------------

	UFUNCTION(BlueprintImplementableEvent)
		void SetDynamicDelegateBind();
	// ex) ���� Ŭ������ ���̳��� ��������Ʈ�� ���ε� �ؾ��Ҷ�.


	//--------------------------------------------------

	// ����ϴ� ���� �ְ� ��������ʴ� ���� �ִ�.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlockMode; // �̰� �ִϸ��̼��� ���� ����.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlocking; // �̰� ���� �� �ܰ����� �˻��ϴ� ����. �̰��� true���� ���� ���� �Ϸ� �ΰ�.
};
