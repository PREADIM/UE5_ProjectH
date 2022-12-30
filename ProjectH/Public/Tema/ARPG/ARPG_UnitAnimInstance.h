// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "ARPG_UnitAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UARPG_UnitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UARPG_UnitAnimInstance();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void Hit();
	void Death();
	void WeaponOnOff(bool bFlag);

	//----------------------------------
	// 노티파이

	UFUNCTION()
		void AnimNotify_BattleMode();
	UFUNCTION()
		void AnimNotify_NormalMode();
	UFUNCTION()
		void AnimNotify_AttackStart();	
	UFUNCTION()
		void AnimNotify_Attack_End();
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
		class UAnimMontage* DeathMontage;

	//----------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsSheathed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
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
		bool bShieldHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsParring;
	
};
