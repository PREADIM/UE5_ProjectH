// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "Tema/ARPG/AttackClass.h"
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

public:
	float SetDirection();

public:
	UPROPERTY(BlueprintReadWrite)
		class AARPGEnermy_Mini* OwnerUnit;

	// ����Ʈ�� ����� AttackClass�� �ش� ���ݸ��� �������ִ°��� ���ݽÿ� ������.
	UPROPERTY(VisibleAnywhere)
		int32 CurrentEffectIndex;
	UPROPERTY(VisibleAnywhere)
		int32 CurrentSoundIndex;
	UPROPERTY(VisibleAnywhere)
		int32 CurrentAttackSoundIndex;

	UPROPERTY(VisibleAnywhere)
		TArray<FAttackEffect> CurrentEffects;
	UPROPERTY(VisibleAnywhere)
		TArray<class USoundBase*> CurrentSounds;
	UPROPERTY(VisibleAnywhere)
		TArray<class USoundBase*> CurrentAttackSounds;

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

	// �и��� �ǽ��� ������ �Ͻ������� �����´�.
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


	bool bPlayedSound; // ���尡 ���������� �Ǵ��ϴ� ����
	FTimerHandle SoundHandle;

	UFUNCTION()
		void PlayedSoundFunc();
	
};
