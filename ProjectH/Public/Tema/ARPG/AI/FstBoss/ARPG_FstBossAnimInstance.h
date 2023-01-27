// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "ARPG_FstBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UARPG_FstBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UARPG_FstBossAnimInstance();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	float SetDirection();

	// 몽타주 실행 함수.
	void PlayAttackMontage(class UAnimMontage* AttackMontage);
	void PlayHitMontage();
	void PlayDeadMontage();
	void PlayParringHitMontage();
	void ZeroAP();


	//-----------------------------------------------------------


	// PlayEffect와 PlaySound는 AttakClass의 SkillNumber를 통해 결정된다.
	UFUNCTION()
		void AnimNotify_PlayEffect();

	UFUNCTION()
		void AnimNotify_PlaySound();

	

	UFUNCTION()
		void AnimNotify_AttackStart();

	UFUNCTION()
		void AnimNotify_ComboSection_End();

	UFUNCTION()
		void AnimNotify_Attack_End();

	UFUNCTION()
		void AnimNotify_ParringHit();

	UFUNCTION()
		void AnimNotify_HitEnd();

	UFUNCTION()
		void AnimNotify_Death();

public:
	UPROPERTY()
		class AARPGEnermy_FstBoss* FstBoss; // Owner

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ZeroAPMontage; // 그로기

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ParringHitMontage;

	// 패링을 실시한 유닛을 일시적으로 가져온다.
	UPROPERTY(BlueprintReadWrite)
		class AARPGUnitBase* ParringInstigatorUnit;
	//-----------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bSupArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAccelerating;	


};
