// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGUnitSkill.h"
#include "JRPGUnit.generated.h"


DECLARE_MULTICAST_DELEGATE(FAIAttackEnd);

UENUM(BlueprintType)
enum class EPlayerType : uint8
{
	Player UMETA(DisplayName = "Player"),
	Enermy UMETA(DisplayName = "Enermy")
};

UCLASS()
class PROJECTH_API AJRPGUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJRPGUnit();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SpringArm, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera3P, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/*----------------------
			AI & BT
	------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BT; 
	class UBehaviorTree* GetBT() { return BT; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AJRPGAIController* OwnerAIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AJRPGGameMode* GM;

	FAIAttackEnd OnAIAttackEnd; // BT���� ������ �Ϸ�Ǹ� ȣ��� ��������Ʈ.

	UFUNCTION(BlueprintCallable)
		void CallAIAttackEnd(); // AI ��������Ʈ ȣ��.

	//----------


	UFUNCTION(BlueprintCallable)
		void AddMPAndULT(); // ���� ����� MP�� ULT �������� ����.
	// ��Ÿ ����� ����ؼ� �Լ��� ���θ��� �ѹ��� ȣ���ϰ� �Ѵ�.
	UFUNCTION(BlueprintCallable)
		void AddManyMPAndULT(); // �ټ� ����
	UFUNCTION(BlueprintCallable)
		void OwnerAddMPAndULT(); // ������ ����� ������ �ñر� �������� ���� �ؾ��Ѵ�.
	// �ñر⸦ ����� ��쿡�� ���� ����. // �Ϲ� �����϶� �� ���.
	UFUNCTION(BlueprintCallable)
		void OwnerAddULT(); // ��ų�� ����� ��쿡�� �ð������� ����.

	UFUNCTION()
		void Forward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void LookUp(float AxisValue);
	UFUNCTION()
		void LookRight(float AxisValue);
	
	UFUNCTION(BlueprintImplementableEvent)
		void TestKey();

	void LMB();

	UFUNCTION(BlueprintCallable)
		void NormalAttack();
	UFUNCTION(BlueprintCallable)
		void Skill_1();
	UFUNCTION(BlueprintCallable)
		void Skill_ULT();


	/* �������Ʈ���� ����. */
	UFUNCTION(BlueprintImplementableEvent)
		void CallLMB();
	UFUNCTION(BlueprintImplementableEvent)
		void CallNormalAttack();
	UFUNCTION(BlueprintImplementableEvent)
		void CallSkill_1();
	UFUNCTION(BlueprintImplementableEvent)
		void CallULT();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void NormalAttackDamage();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SkillAttackDamage();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ULTAttackDamage();
	/* �� �Լ����� ĳ������ ������ ����� �ٸ��Ƿ� ��� ����Ʈ���� �������. */

	UFUNCTION(BlueprintCallable)
		void UnitTurnEnd();

	UFUNCTION(BlueprintCallable)
		void AttackEnd(); // ĳ������ ������ ��������
	

public:
	void BattleStart(bool bFlag);
	void OwnerUnitBattleStart();
	void EnermyBattleStart();

	void InitCurrentStat(); // ���� ü�°� MP�� �����´�.

	UFUNCTION(BlueprintCallable)
		void TargetAttack(float ATK); // �ϳ��� ������.
	UFUNCTION(BlueprintCallable)
		void TargetManyAttack(float ATK); // ���� ������ ������.

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AJRPGPlayerController* OwnerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UJRPGComponent* BattleComponent;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EPlayerType PlayerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* LMBAnim;


	UPROPERTY(VisibleAnywhere)
		float MouseSensitivity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 CharNum; // �ش� ĳ������ �ѹ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EnermyLevel = 1; // (�� ���� ����) ���� ����. ������ ���̺� �˻��ؼ� ������ �������� �����̴�.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJRPGUnitSkill UnitSkills;

	

	/*-----------------
		���� �����̽�
	-------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JRPGUnit)
		bool bIsJRPGUnit = false; // �ش� ĳ���Ͱ� JRPG �������� ������ ĳ�������� Ȯ���ϴ� ����. 
	// �� ������ �����̴� �ִϸ��̼��� �ٲ۴�.
	// �� ������ ���� ������ ��� ���� ������ �������� ���ϰ� ���ش�.
	UFUNCTION(BlueprintImplementableEvent)
		void SetIsJRPGUnit(bool bFlag); // �� ������ �¾��� �Լ�. �ش� �Լ��� ���� �ִԺ��� ������ �ٲ�.
	void ThisUnitBattleUnit(bool bFlag); // c++���� �̰ɷ� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
		class UWidgetComponent* BattleHPComponent;

	UPROPERTY()
		class UJRPGBattleHPWidget* BattleHPWidget;

	void BattleWidgetOnOff(bool bOnOff);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattlePriority)
		int32 Priority; // �켱����


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJRPGCharStat CharacterStat; // �ش� ĳ������ ����
	// �÷��̾�� Save & Load�� ������, ���� �������Ʈ���� ������.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentHP; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentMP; 

	UPROPERTY(BlueprintReadWrite)
		float CurrentExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ULTGage; // �ñر� ������.

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxULTGage; // �ִ� �ñر� ������.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsLMBAttack;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* DeadAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* HitAnim;


};
