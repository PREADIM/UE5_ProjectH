// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Tema/JRPG/JRPGCharStat.h"
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


	void LMB();

	UFUNCTION(BlueprintCallable)
		void NormalAttack();
	UFUNCTION(BlueprintCallable)
		void Skill_1();
	UFUNCTION(BlueprintCallable)
		void Skill_2();
	UFUNCTION(BlueprintCallable)
		void Skill_3();


	/* �������Ʈ���� ����. */

	UFUNCTION(BlueprintImplementableEvent)
		void CallNormalAttack();
	UFUNCTION(BlueprintImplementableEvent)
		void CallSkill_1();
	UFUNCTION(BlueprintImplementableEvent)
		void CallSkill_2();
	UFUNCTION(BlueprintImplementableEvent)
		void CallSkill_3();

public:
	void BattleStart();


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AJRPGPlayerController* OwnerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UJRPGComponent* BattleComponent;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EPlayerType PlayerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* LMBAnim;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> EnermyUnits; // �� ���ָ� ����ϴ� ���, ���� ������������ �ִ��� �Ǵ�.


	UPROPERTY(VisibleAnywhere)
		float MouseSensitivity;

	/*-----------------
		���� �����̽�
	-------------------*/
	UPROPERTY()
		int32 Priority; // �켱����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CharacterNum; // ĳ������ ��������� ���� �ѹ�. �̰ɷ� ���� ������ �ذ��� ������.

	FJRPGCharStat CharacterStat; // �ش� ĳ������ ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentMP;


};
