// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "Tema/ARPG/AttackClass.h"
#include "PhysicalSoundStruct.h"
#include "ARPG_FstBossAnimInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESFXMode_FstBoss : uint8
{
	Default UMETA(DisplayName = "Default"),
	HitSound UMETA(DisplayName = "HitSound"),
	Death UMETA(DisplayName = "Death")
};


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


	void HittedReset(); // 공격도중 맞아서 초기화 해야하는 변수들을 초기화

	//-----------------------------------------------------------

	/*Attack Class마다 추가로 필요한 노티파이*/
	//ComboAttack에서 날라갈 투사체
	UFUNCTION()
		void AnimNotify_ComboProjectile();

	// 텔레포트 
	UFUNCTION()
		void AnimNotify_FstBoss_Teleport();

	UFUNCTION()
		void AnimNotify_FstBoss_TeleportEnd();


	// R 어택 투사체를 빙자한 콜리전 소환
	UFUNCTION()
		void AnimNotify_FstBoss_R_Projectile();


	// E 어택 투사체를 빙자한 콜리전 소환
	UFUNCTION()
		void AnimNotify_FstBoss_E_Projectile();


	//--------------------------------------------------------------


	UFUNCTION()
		void AnimNotify_PlayEffect();

	UFUNCTION()
		void AnimNotify_PlaySound();

	UFUNCTION()
		void AnimNotify_PlayAttackSound();

	UFUNCTION()
		void AnimNotify_TwinAttackStart();

	UFUNCTION()
		void AnimNotify_LeftAttackStart();

	UFUNCTION()
		void AnimNotify_RightAttackStart();

	UFUNCTION()
		void AnimNotify_TwinAttackEnd();

	UFUNCTION()
		void AnimNotify_LeftAttackEnd();

	UFUNCTION()
		void AnimNotify_RightAttackEnd();

	UFUNCTION()
		void AnimNotify_Attack_End();

	UFUNCTION()
		void AnimNotify_ParringHit();

	UFUNCTION()
		void AnimNotify_HitEnd();

	UFUNCTION()
		void AnimNotify_Death();

	UFUNCTION()
		void AnimNotify_DontMoving();

	UFUNCTION()
		void AnimNotify_DontLockOn();

	UFUNCTION()
		void AnimNotify_CanLockOn();

	UFUNCTION()
		void AnimNotify_HitSound(); // Hit몽타주 실행시 히트 사운드가 나오지만, 따로 애니메이션에서 추가로 호출해야할때 호출.

	UFUNCTION()
		void AnimNotify_SpecialAttackEnd();

	//----------------------------------------------------

	void FootStepPlaySound(int32 SoundNum);

	UFUNCTION()
		void AnimNotify_WalkSound();

	UFUNCTION()
		void AnimNotify_SprintSound();

	UFUNCTION()
		void AnimNotify_JumpSound();


public:
	UPROPERTY()
		class AARPGEnermy_FstBoss* FstBoss; // Owner

	//----------------------------------
	// AttackClass에서 받아와서 현재 실행해야할 이펙트와 사운드가 무엇인지 판단하는 변수들

	UPROPERTY(VisibleAnywhere)
		int32 CurrentEffectIndex;
	UPROPERTY(VisibleAnywhere)
		int32 CurrentSoundIndex;
	UPROPERTY(VisibleAnywhere)
		int32 CurrentAttackSoundIndex;

	UPROPERTY(VisibleAnywhere)
		TArray<FAttackEffect> CurrentEffects;
	UPROPERTY(VisibleAnywhere)
		TArray<FSoundAndAttenuation> CurrentSounds;
	UPROPERTY(VisibleAnywhere)
		TArray<FSoundAndAttenuation> CurrentAttackSounds;

	//----------------------------------
	//SFX

	UPROPERTY(EditAnywhere)
		TMap<ESFXMode_FstBoss, FSoundAndAttenuation> SFXSounds;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* DeathEffect;


	//----------------------------------



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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 ProjectileCnt = 0; // 투사체 변수


	bool bPlayedSound; // 사운드가 실행중인지 판단하는 변수
	FTimerHandle SoundHandle;
	UFUNCTION()
		void PlayedSoundFunc();
};
