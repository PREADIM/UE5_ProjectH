// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "Tema/ARPG/AttackClass.h"
#include "PhysicalSoundStruct.h"
#include "ARPG_EnermyAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEnermy_Mini_Mode : uint8
{
	BattleMode UMETA(DisplayName = "BattleMode"),
	BlockingMode UMETA(DisplayName = "BlockingMode")
};


UENUM(BlueprintType)
enum class ESFXMode_Enermy_Mini : uint8
{
	Default UMETA(DisplayName = "Default"),
	HitSound UMETA(DisplayName = "HitSound"),
	BlockingHitSound UMETA(DisplayName = "BlockingHitSound"),
	Death UMETA(DisplayName = "Death")
};


UCLASS()
class PROJECTH_API UARPG_EnermyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UARPG_EnermyAnimInstance();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	

	void PlayAttackMontage(class UAnimMontage* AttackMontage);
	void PlayBlockingMontage();
	void PlayHitMontage(EEnermy_Mini_Mode UnitMode);
	void PlayDeadMontage();
	void PlayParringHitMontage();
	void ZeroAP();


	void SpecialAttackHitMontagePlay();

public:
	float SetDirection();

public:
	UPROPERTY(BlueprintReadWrite)
		class AARPGEnermy_Mini* OwnerUnit;

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
		TMap<ESFXMode_Enermy_Mini, FSoundAndAttenuation> SFXSounds;

	//----------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* BlockingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* BlockingHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* BlockingZeroAPMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ParringHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* SpecialAttackHitMontage;

	// 패링을 실시한 유닛을 일시적으로 가져온다.
	UPROPERTY(BlueprintReadWrite)
		class AARPGUnitBase* ParringInstigatorUnit;


	//-----------------------------------------------------------

	UFUNCTION()
		void AnimNotify_PlayEffect();

	UFUNCTION()
		void AnimNotify_PlaySound();

	UFUNCTION()
		void AnimNotify_PlayAttackSound();

	UFUNCTION()
		void AnimNotify_AttackStart();

	UFUNCTION()
		void AnimNotify_ComboSection_End();

	UFUNCTION()
		void AnimNotify_Attack_End();

	UFUNCTION()
		void AnimNotify_ParringHit();

	UFUNCTION()
		void AnimNotify_BlockStart();

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

	//-----------------------------------------------------------

	void FootStepPlaySound(int32 SoundNum);

	UFUNCTION()
		void AnimNotify_WalkSound();

	UFUNCTION()
		void AnimNotify_SprintSound();

	UFUNCTION()
		void AnimNotify_JumpSound();

	//-----------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bParring;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bHitting;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bSupArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBattleMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Direction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAccelerating;


	bool bPlayedSound; // 사운드가 실행중인지 판단하는 변수
	FTimerHandle SoundHandle;

	UFUNCTION()
		void PlayedSoundFunc();
	
};
