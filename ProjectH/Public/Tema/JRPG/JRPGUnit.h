// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGUnitSkill.h"
#include "PhysicalSoundStruct.h"
#include "Tema/JRPG/DebuffClass.h"
#include "JRPGUnit.generated.h"


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
	AJRPGUnit();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SpringArm)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera3P)
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterName)
		FString CharacterName;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
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

	//--------------------------------------------------


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
	// ĳ���͵� �������� ��ų �� ����
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
		void AttackEnd(); // ĳ������ ������ �������� ��������

public:
	void BattleStart(bool bFlag);
	void OwnerUnitBattleStart();
	void EnermyBattleStart();

	void InitCurrentStat(); // ���� ü�°� MP�� �����´�.

	UFUNCTION(BlueprintCallable)
		void TargetAttack(float ATK, TSubclassOf<class UDebuffClass> BP_DebuffClass = nullptr); // �ϳ��� ������.
	UFUNCTION(BlueprintCallable)
		void TargetManyAttack(float ATK, TSubclassOf<class UDebuffClass> BP_DebuffClass = nullptr); // ���� ������ ������.


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AJRPGPlayerController* OwnerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UAnimInstance* AnimInstance;

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


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WalkSpeed;
	

	/*-----------------
		���� �����̽�
	-------------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = JRPGUnit)
		bool bIsJRPGUnit = false; // �ش� ĳ���Ͱ� JRPG �������� ������ ĳ�������� Ȯ���ϴ� ����. 
	// �� ������ �����̴� �ִϸ��̼��� �ٲ۴�.
	// �� ������ ���� ������ ��� ���� ������ �������� ���ϰ� ���ش�.

	UFUNCTION(BlueprintImplementableEvent)
		void SetIsJRPGUnit(bool bFlag); // �� ������ �¾��� �Լ�. �ش� �Լ��� ���� �ִԺ��� ������ �ٲ�.

	void ThisUnitBattleUnit(bool bFlag); // c++���� �̰ɷ� ����

	void BattleWidgetOnOff(bool bOnOff);


	//---------------------------------------------------------
	/*-------------------------
			�߿� �������ͽ�
	--------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
		class UWidgetComponent* BattleHPComponent;
	UPROPERTY()
		class UJRPGBattleHPWidget* BattleHPWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterStat)
		int32 Priority; // �켱����

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterStat)
		FJRPGCharStat CharacterStat; // �ش� ĳ������ ����

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterStat)
		float CurrentHP; 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterStat)
		float CurrentMP; 

	UPROPERTY(BlueprintReadWrite, Category = CharacterStat)
		float CurrentExp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterStat)
		float CurrentULTGage; // �ñر� ������.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float MaxULTGage; // �ִ� �ñر� ������.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterStat)
		bool bIsLMBAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* DeadAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* HitAnim;

	//-----------------------------------------------
	/*-----------------------------------
		���� ���� ���� �� ����� ���� �Լ�
	----------------------------------*/
	UFUNCTION(BlueprintCallable)
		void SetStatDEF(float DEF);
	UFUNCTION(BlueprintCallable)
		void SetStatATK(float ATK);


	//-------------------------------------------
	/*-----------------------
			������ ����
	------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EPhysicalSurface>, FPhysicalSoundStruct> PhysicalAllSounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FPhysicalSoundStruct PhysicalSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SurfaceDistance = 150.f; // ������ �Ÿ�
	void SetPhysicalSound();

	//-------------------------------------------
	/*-----------------------
		���� ��ŸƮ ��Ÿ��
	------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* BattleStartMontage;
	void PlayStartMontage();


	/*-----------------------
		CC�� �� ����� ����
	------------------------*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSet<FDebuffStruct> DebuffSet;

	UFUNCTION(BlueprintCallable)
		void SetCCState(ECCType CCType, bool bFlag);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bCC;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FCCState CCState;
};
