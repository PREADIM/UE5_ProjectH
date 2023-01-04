// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "ARPG_UnitAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EUnitMode : uint8
{
	BattleMode UMETA(DisplayName="BattleMode"),
	BlockingMode UMETA(DisplayName="BlockingMode")
};


UCLASS()
class PROJECTH_API UARPG_UnitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UARPG_UnitAnimInstance();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void Hit(EUnitMode UnitMode);
	void Death();
	void WeaponOnOff(bool bFlag);
	void ZeroAP();

	//----------------------------------
	// 노티파이

	UFUNCTION()
		void AnimNotify_BattleMode();
	UFUNCTION()
		void AnimNotify_NormalMode();
	UFUNCTION()
		void AnimNotify_AttackStart();	
	UFUNCTION()
		void AnimNotify_ComboSection_End();
	UFUNCTION()
		void AnimNotify_Attack_End();
	UFUNCTION()
		void AnimNotify_BlockStart();
	UFUNCTION()
		void AnimNotify_HitEnd();
	UFUNCTION()
		void AnimNotify_ParringStart();
	UFUNCTION()
		void AnimNotify_ParringEnd();

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
		class UAnimMontage* ParringHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* DeathMontage;

	//----------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsSheathed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool AttackingLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool AttackingRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool AttackingForward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool AttackingBackward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bHitting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsParring;
	
};
