// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "Tema/ARPG/AttackClass.h"
#include "ARPG_TPSAnimInstance.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EUnitMode : uint8
{
	BattleMode UMETA(DisplayName = "BattleMode"),
	BlockingMode UMETA(DisplayName = "BlockingMode")
};


UCLASS()
class PROJECTH_API UARPG_TPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UARPG_TPSAnimInstance();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void Parring();
	void Hit(EUnitMode UnitMode);
	void Death();
	void WeaponOnOff(bool bFlag);
	void ShieldZeroAP();
	void ParringAttack();

	float GetDirection();

	//----------------------------------
	// 노티파이

	UFUNCTION()
		void AnimNotify_BattleMode();
	UFUNCTION()
		void AnimNotify_NormalMode();

	//----------------------------------


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnit* OwnerUnit;

	//----------------------------------
	// 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* WeaponOpenMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* WeaponCloseMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* BlockingHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* BlockingZeroAPMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ParringMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ParringHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ParringAttackMontage;

	//----------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsSprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsSheathed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AttackingLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AttackingRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AttackingForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AttackingBackward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Direction;

};
