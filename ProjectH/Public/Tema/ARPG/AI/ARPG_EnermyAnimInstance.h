// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "ARPG_EnermyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UARPG_EnermyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UARPG_EnermyAnimInstance();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	
	//void PlayGuardMontage();
	void PlayAttackMontage(class UAnimMontage* AttackMontage);
	void PlayHitMontage();
	void PlayDeadMontage();

public:
	float SetDircection();

public:
	UPROPERTY(BlueprintReadWrite)
		class AARPGEnermy_Mini* OwnerUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* DeadMontage;

	//-----------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bParring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bHitting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBattleMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Direction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAccelerating;
	
};
