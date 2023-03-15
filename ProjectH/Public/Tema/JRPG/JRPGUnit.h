// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGUnitSkill.h"
#include "PhysicalSoundStruct.h"
#include "Tema/JRPG/DebuffClass.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/AIAsyncTaskBlueprintProxy.h"
#include "JRPGUnit.generated.h"


UENUM(BlueprintType)
enum class EPlayerType : uint8
{
	Player UMETA(DisplayName = "Player"),
	Enermy UMETA(DisplayName = "Enermy")
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	Normal UMETA(DisplayName = "Normal"),
	Skill UMETA(DisplayName = "Skill"),
	ULT UMETA(DisplayName = "ULT")
};

USTRUCT(BlueprintType)
struct FAttackSFX
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class USoundBase*> AttackSounds;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BattleCollision)
		class USphereComponent* OverlapBattleStartCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterName)
		FString CharacterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WalkSpeed)
		float NormalWalkSpeed = 450.f; 

protected:
	virtual void BeginPlay();

public:	
	/*----------------------
			virtual 
	------------------------*/

	virtual void Tick(float DeltaTime);
	virtual void PossessedBy(AController* NewController);
	virtual void PostInitializeComponents();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	virtual void BattleTurnStart() {}
	virtual void UnitBattleStart();
	virtual void DeadBattleListRemove() {}

	/*---------------------------------------------*/

	void TakeDamageCalculator(float DamageAmount); // ������ ���

	/*----------------------
			AI & BT
	------------------------*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AJRPGAIController* BattleAIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BT; 
	class UBehaviorTree* GetBT() { return BT; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AJRPGGameMode* GM;

	//--------------------------------------------------

	UFUNCTION(BlueprintCallable)
		void TargetAddMPAndULT(); // ���� ����� MP�� ULT �������� ����.
	// ��Ÿ ����� ����ؼ� �Լ��� ���θ��� �ѹ��� ȣ���ϰ� �Ѵ�.
	UFUNCTION(BlueprintCallable)
		void ManyTargetsAddMPAndULT(); // �ټ� ����
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

	/*--------------------------------
			���� ���� �������̽�
	----------------------------------*/

	UFUNCTION(BlueprintCallable)
		void NormalAttack();
	UFUNCTION(BlueprintCallable)
		void Skill_1();
	UFUNCTION(BlueprintCallable)
		void Skill_ULT();


	/* �������Ʈ���� ����. */
	// ĳ���͵� �������� ��ų �� ����
	UFUNCTION(BlueprintImplementableEvent)
		void CallNormalAttack();
	UFUNCTION(BlueprintImplementableEvent)
		void CallSkill_1();
	UFUNCTION(BlueprintImplementableEvent)
		void CallULT();

	/* �� �Լ����� ĳ������ ������ ����� �ٸ��Ƿ� ��� ����Ʈ���� �������. */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void NormalAttackDamage();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SkillAttackDamage();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ULTAttackDamage();

	

	/* �̵��ؼ� �����ϴ� ���ݷ� ���� ���� �Լ�. */

	EAttackType MoveToAttackType;
	UFUNCTION(BlueprintCallable)
		void MoveToAttack(float MoveSpeed, EAttackType AttackType); // �������� �����ؾ��ϴ� ĳ������ ��� �� �Լ��� ����.
	UFUNCTION()
		void MoveToPlayMontage(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	UFUNCTION()
		void AttackMontagePlay();
	UFUNCTION()
		void MoveToDefaultLocation();
	UFUNCTION()
		void MoveToDefaultLocationEnded(FAIRequestID RequestID, EPathFollowingResult::Type Result);


	/* �� ���� ���� */

	/* ĳ������ ������ �������� �������� */
	UFUNCTION(BlueprintCallable)
		void AttackEnd(); 
	/* ������ ������ Ÿ�� ���ֿ��� ������ ������ [���� Ÿ��, �Ѹ� or ������, �����ϴ� Ÿ��, ����� Ŭ���� */
	UFUNCTION(BlueprintCallable)
		void TargetAttackDamageProxy(EAttackType AttackType, bool bOnce, int32 AttackCnt = 1, TSubclassOf<class UDebuffClass> BP_DebuffClass = nullptr);
	/* �������� ���� �� �� �����ϴ� �� [�ϳ����� ����������, ������ ������, �����Ḧ ���� ������] */
	UFUNCTION(BlueprintCallable)
		void DamagedTurnEnd(bool bOnce, bool bTurnEnd, float Delay = 0.5f); 
	/* �ݵ�� ȣ�� �Ǿ�� �� �� ���� */
	UFUNCTION(BlueprintCallable)
		void UnitTurnEnd();

	//------------------------------------------------------

	UFUNCTION()
		void BattleStartCollisionBeginOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodtIndex, bool bFromSweep, const FHitResult& HitResult);

public:
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = JRPGUnit)
		bool bIsJRPGUnit = false; // �ش� ĳ���Ͱ� JRPG �������� ������ ĳ�������� Ȯ���ϴ� ����. 
	// �� ������ �����̴� �ִϸ��̼��� �ٲ۴�.
	// �� ������ ���� ������ ��� ���� ������ �������� ���ϰ� ���ش�.
	UFUNCTION(BlueprintImplementableEvent)
		void SetIsJRPGUnit(bool bFlag); // �� ������ �¾��� �Լ�. �ش� �Լ��� ���� �ִԺ��� ������ �ٲ�.

	void ThisUnitBattleUnit(bool bFlag); // c++���� �̰ɷ� ����
	void BattleWidgetOnOff(bool bOnOff);


	FVector BattleDefaultLocation; // ù �ڸ�

	/*-------------------------
			�߿� �������ͽ�
	--------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleWidget)
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


	/*------------------
			��Ÿ��
	--------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
		class UAnimMontage* DeadAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
		class UAnimMontage* HitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackMontage)
		class UAnimMontage* NormalAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackMontage)
		class UAnimMontage* SkillMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackMontage)
		class UAnimMontage* ULTMontage;

	/*---------------------
			��ų ����
	-----------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackSFX)
		TMap<EAttackType, FAttackSFX> AttackSFXs;
	int32 SFXCnt = 0;

	UFUNCTION(BlueprintCallable)
		void InitSFX();
	UFUNCTION(BlueprintCallable)
		void PlaySFX(EAttackType AttackType);

	/*-----------------------------------
		���� ���� ���� �� ����� ���� �Լ�
	-------------------------------------*/
	UFUNCTION(BlueprintCallable)
		void SetStatDEF(float DEF);
	UFUNCTION(BlueprintCallable)
		void SetStatATK(float ATK);

	/*-----------------------
			������ ����
	------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicalSound)
		TMap<TEnumAsByte<EPhysicalSurface>, FPhysicalSoundStruct> PhysicalAllSounds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PhysicalSound)
		FPhysicalSoundStruct PhysicalSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicalSound)
		float SurfaceDistance = 300.f; // ������ �Ÿ�
	void SetPhysicalSound();

	/*-----------------------
		ĳ���� ���� ��Ÿ��
	------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Montage)
		class UAnimMontage* BattleStartMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
		class UAnimMontage* CharacterChangeMontage;

	void PlayStartMontage();
	void PlayCharacterChangeMontage();


	/*-----------------------
		CC�� �� ����� ����
	------------------------*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Debuff)
		TSet<FDebuffStruct> DebuffSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Debuff)
		bool bAnimCCEnd; // �ִϸ��̼� ��

	UPROPERTY(VisibleAnywhere, Category = Debuff)
		bool bCC;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Debuff)
		FCCState CCState;

	UFUNCTION(BlueprintCallable)
		void SetCCState(ECCType CCType, bool bFlag);

	void UnitTurnEndCCState(); // UnitTurnEnd�� �������� CC�⿡ �ɷ� ���� �� ���Ḧ �ؾ��ϴ� ���.
};
