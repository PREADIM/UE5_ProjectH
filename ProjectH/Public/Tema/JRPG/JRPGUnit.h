// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGUnitSkill.h"
#include "PhysicalSoundStruct.h"
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
	virtual void PostInitializeComponents() override;

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

	FAIAttackEnd OnAIAttackEnd; // BT에서 공격이 완료되면 호출될 델리게이트.

	UFUNCTION(BlueprintCallable)
		void CallAIAttackEnd(); // AI 델리게이트 호출.

	//----------


	UFUNCTION(BlueprintCallable)
		void AddMPAndULT(); // 맞은 대상의 MP와 ULT 게이지가 찬다.
	// 연타 기술을 대비해서 함수로 따로만들어서 한번만 호출하게 한다.
	UFUNCTION(BlueprintCallable)
		void AddManyMPAndULT(); // 다수 공격
	UFUNCTION(BlueprintCallable)
		void OwnerAddMPAndULT(); // 공격한 사람도 마나와 궁극기 게이지가 차긴 해야한다.
	// 궁극기를 사용한 경우에는 차지 않음. // 일반 공격일때 만 사용.
	UFUNCTION(BlueprintCallable)
		void OwnerAddULT(); // 스킬을 사용한 경우에는 궁게이지만 찬다.

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


	/* 블루프린트에서 실행. */
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
	/* 이 함수들은 캐릭마다 데미지 계산이 다르므로 블루 프린트에서 계산해줌. */

	UFUNCTION(BlueprintCallable)
		void UnitTurnEnd();

	UFUNCTION(BlueprintCallable)
		void AttackEnd(); // 캐릭터의 공격이 끝났을때
	

public:
	void BattleStart(bool bFlag);
	void OwnerUnitBattleStart();
	void EnermyBattleStart();

	void InitCurrentStat(); // 현재 체력과 MP를 가져온다.

	UFUNCTION(BlueprintCallable)
		void TargetAttack(float ATK); // 하나만 때린다.
	UFUNCTION(BlueprintCallable)
		void TargetManyAttack(float ATK); // 여러 마리를 때린다.


	// LMB 어택일때 블루프린트에서 실행할 것들.
	UFUNCTION(BlueprintImplementableEvent)
		void PlayLMBAttack();

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
		int32 CharNum; // 해당 캐릭터의 넘버

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EnermyLevel = 1; // (적 유닛 한정) 적의 레벨. 데이터 테이블에 검색해서 스탯을 가져오기 위함이다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJRPGUnitSkill UnitSkills;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WalkSpeed;
	

	/*-----------------
		전투 스테이스
	-------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JRPGUnit)
		bool bIsJRPGUnit = false; // 해당 캐릭터가 JRPG 유닛으로 스폰된 캐릭터인지 확인하는 변수. 
	// 이 변수로 움직이는 애니메이션을 바꾼다.
	// 이 변수로 턴제 유닛인 경우 위젯 사이즈 스케일을 변하게 해준다.
	UFUNCTION(BlueprintImplementableEvent)
		void SetIsJRPGUnit(bool bFlag); // 위 변수를 셋업할 함수. 해당 함수를 통해 애님블프 설정도 바꿈.
	void ThisUnitBattleUnit(bool bFlag); // c++에서 이걸로 실행

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
		class UWidgetComponent* BattleHPComponent;

	UPROPERTY()
		class UJRPGBattleHPWidget* BattleHPWidget;

	void BattleWidgetOnOff(bool bOnOff);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattlePriority)
		int32 Priority; // 우선순위

	//★★★★
	// 디버프 클래스를 받아오자.


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJRPGCharStat CharacterStat; // 해당 캐릭터의 스텟
	// 플레이어는 Save & Load를 하지만, 적은 블루프린트에서 정해줌.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentHP; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentMP; 

	UPROPERTY(BlueprintReadWrite)
		float CurrentExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ULTGage; // 궁극기 게이지.

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxULTGage; // 최대 궁극기 게이지.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsLMBAttack;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* DeadAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* HitAnim;


	// 피지컬 머터리얼 사운드

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EPhysicalSurface>, FPhysicalSoundStruct> PhysicalAllSounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FPhysicalSoundStruct PhysicalSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SurfaceDistance = 150.f; // 땅끝의 거리

	void SetPhysicalSound();

	// 레벨 스타트 몽타주
	UPROPERTY(EditAnywhere)
		class UAnimMontage* BattleStartMontage;
	void PlayStartMontage();

};
