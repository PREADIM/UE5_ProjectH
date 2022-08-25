// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGUnitSkill.h"
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


	bool GetUsingSkill() { return bUsingSkill; }
	UFUNCTION(BlueprintImplementableEvent)
		void UnitSkillESC();


public:
	void BattleStart();
	void EnermyBattleStart();

	void InitCurrentStat(); // 현재 체력과 MP를 가져온다.

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

	UPROPERTY(VisibleAnywhere)
		int32 CharNum; // 해당 캐릭터의 넘버

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJRPGUnitSkill UnitSkills;

	/*-----------------
		전투 스테이스
	-------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattlePriority)
		int32 Priority; // 우선순위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CharacterNum; // 캐릭터의 모든정보를 가진 넘버. 이걸로 거의 모든것을 해결할 수있음.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJRPGCharStat CharacterStat; // 해당 캐릭터의 스텟
	// 플레이어는 Save & Load를 하지만, 적은 블루프린트에서 정해줌.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentHP; // 적은 미리 정해주자.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentMP; // 적은 미리 정해줌.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float ULTGage; // 궁극기 게이지.

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxULTGage; // 최대 궁극기 게이지.


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsLMBAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUsingSkill; // 스킬 실행중. 스킬실행중이면 ESC로 끄기 위해서.


};
