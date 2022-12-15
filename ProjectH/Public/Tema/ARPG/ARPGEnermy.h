// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
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
		bool bMoving; // �� ������ �����̰ų� �齺���� ��.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BattleSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PrevSpeed; // �鹫���� �Ҷ��� �ӵ��� �پ�� �ϹǷ� ��� �����


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EMovingState EnermyMoveState; // ���� �Է� �Ǵ���.

	// ���� ��� �������� �ش� ������ EnermyMoveState�� �����Ѵ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EEnermyMoveMode State = EEnermyMoveMode::None;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentCollisionRadius; // ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalCollisionRadius; // ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BattleCollisionRadius; // ��Ʋ ���� ����

	// �� ������ ���� �¼��� ���� ����. ������ �ٸ��⶧���� �� ���� �ǽð����� �����ͼ� �ٲ۴�.
	// ������ ��쿡�� ������ ���� -> ���� �̹Ƿ� �ʿ����� �������� ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float BattleDistance; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalDistance;

	// ���� �Ÿ��� ��Ÿ���� ���� ������ �ٸ��⶧���� ���� �� ���� ���÷� �ٲ��ش�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackDistance;

	float GetAttackDistance() { return AttackDistance; }


	UPROPERTY()
		class AARPGAIController* OwnerAIController;

	UPROPERTY(BlueprintReadOnly)
		class UARPG_EnermyAnimInstance* EnermyAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnit* PlayerUnit; // BT���� �������Ͽ� ������ Ÿ��.


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BT;
	class UBehaviorTree* GetBT() { return BT; }

public:
	virtual void Attack() {} // ���� �Լ�
	virtual void Garud() {} // ���� �Լ�


	UFUNCTION(BlueprintImplementableEvent)
		void EnermyMoving(); // �������Ʈ���� �� ���� �˸´� �ൿ �ϰ��ϱ� �������� Tick���� ������ ���� �Լ�.

	UFUNCTION(BlueprintCallable)
		FVector GetMovingValue(); // �����̴��� Value�� ���س���

	UFUNCTION(BlueprintCallable)
		void LockOnPlayer(); // SetActorRotation���� �÷��̾� �����ϴ� �Լ�.

	UFUNCTION(BlueprintImplementableEvent)
		void ParringMontage();

	UFUNCTION(BlueprintCallable)
		float GetDirection(FVector MoveDirection); // ���� � �������� �ִԺ����� �����ϴ� �Լ�.


	float GetCollisionRadius() { return CurrentCollisionRadius; }
	void SetCollisionRadius(bool bFlag); // �÷��̾ ã������ ���������� �þ��, �ƴѰ��� ���� �������� (���� �������)
	

	float GetBattleDistance() { return BattleDistance; }
	void SetBattleDistance(float Distance) { BattleDistance = Distance; }

	void SetBlocking(bool bFlag); // BT���� ���и� ����� ��������
	void SetBattleMode(bool bFlag); // BT���� ���и� ����� ��������

	void SetEnermyMoveMode(EEnermyMoveMode Mode); // ������ ��� ����

	void CastMoving();
	//������ ������ ĳ��Ʈ�Ͽ� ������ �� �����ٴ°� �˷��ִ� �Լ�.
};
