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

	// ��Ÿ�� ���� �Լ�.
	void PlayAttackMontage(class UAnimMontage* AttackMontage);
	void PlayHitMontage();
	void PlayDeadMontage();
	void PlayParringHitMontage();
	void ZeroAP();


	void HittedReset(); // ���ݵ��� �¾Ƽ� �ʱ�ȭ �ؾ��ϴ� �������� �ʱ�ȭ

	//-----------------------------------------------------------

	/*Attack Class���� �߰��� �ʿ��� ��Ƽ����*/
	//ComboAttack���� ���� ����ü
	UFUNCTION()
		void AnimNotify_ComboProjectile();

	// �ڷ���Ʈ 
	UFUNCTION()
		void AnimNotify_FstBoss_Teleport();

	UFUNCTION()
		void AnimNotify_FstBoss_TeleportEnd();


	// R ���� ����ü�� ������ �ݸ��� ��ȯ
	UFUNCTION()
		void AnimNotify_FstBoss_R_Projectile();


	// E ���� ����ü�� ������ �ݸ��� ��ȯ
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
		void AnimNotify_HitSound(); // Hit��Ÿ�� ����� ��Ʈ ���尡 ��������, ���� �ִϸ��̼ǿ��� �߰��� ȣ���ؾ��Ҷ� ȣ��.

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
	// AttackClass���� �޾ƿͼ� ���� �����ؾ��� ����Ʈ�� ���尡 �������� �Ǵ��ϴ� ������

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
		class UAnimMontage* ZeroAPMontage; // �׷α�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ParringHitMontage;

	// �и��� �ǽ��� ������ �Ͻ������� �����´�.
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
		int32 ProjectileCnt = 0; // ����ü ����


	bool bPlayedSound; // ���尡 ���������� �Ǵ��ϴ� ����
	FTimerHandle SoundHandle;
	UFUNCTION()
		void PlayedSoundFunc();
};
