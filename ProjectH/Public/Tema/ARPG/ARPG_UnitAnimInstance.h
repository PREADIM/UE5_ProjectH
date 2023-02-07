// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "Tema/ARPG/ARPG_TPSAnimInstance.h"
#include "ARPG_UnitAnimInstance.generated.h"


UENUM(BlueprintType)
enum class ESFXMode : uint8
{
	Default UMETA(DisplayName = "Default"),
	DrawSword UMETA(DisplayName = "DrawSword"),
	StowSword UMETA(DisplayName = "StowSword"),
	AttackSound UMETA (DisplayName = "AttackSound"),
	HitSound UMETA(DisplayName = "HitSound"),
	BlockingHitSound UMETA(DisplayName = "BlockingHitSound"),
	Death UMETA(DisplayName = "Death")
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
	void Parring();
	void Hit(EUnitMode UnitMode);
	void Death();
	void WeaponOnOff(bool bFlag);
	void ShieldZeroAP();
	void ParringAttack();

	UPROPERTY(VisibleAnywhere)
		bool bMontagePlaying;
	UFUNCTION()
		void MontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void MontageStarted(UAnimMontage* Montage);

	//----------------------------------
	// ��Ƽ����

	UFUNCTION()
		void AnimNotify_PlayEffect();

	UFUNCTION()
		void AnimNotify_PlaySound();


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
	UFUNCTION()
		void AnimNotify_ParringPlayingEnd();
	UFUNCTION()
		void AnimNotify_SprintStart();

	UFUNCTION()
		void AnimNotify_UseAP();
	UFUNCTION()
		void AnimNotify_HoldAP();
	UFUNCTION()
		void AnimNotify_HoldAttack();


	//�и� ���� �� ���� �ý��� ( ������ ������ �־���� )
	UFUNCTION()
		void AnimNotify_SpecialAttackEnd();

	UFUNCTION()
		void AnimNotify_Death();


	// �� �ȱ� �ٱ� ���� ���� ����Ʈ

	void FootStepPlaySound(int32 SoundNum);	

	UFUNCTION()
		void AnimNotify_WalkSound();

	UFUNCTION()
		void AnimNotify_SprintSound();

	UFUNCTION()
		void AnimNotify_JumpSound();

	//----------------------------------


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnit* OwnerUnit;

	//----------------------------------
	// ����Ʈ ���� AttackClass���� �����ͼ� ��Ƽ���̿��� ����

	UPROPERTY(VisibleAnywhere)
		int32 CurrentEffectIndex;
	UPROPERTY(VisibleAnywhere)
		int32 CurrentSoundIndex;
	UPROPERTY(VisibleAnywhere)
		TArray<FAttackEffect> CurrentEffects;
	UPROPERTY(VisibleAnywhere)
		TArray<class USoundBase*> CurrentSounds;

	//----------------------------------
	//SFX

	UPROPERTY(EditAnywhere)
		TMap<ESFXMode, class USoundBase*> SFXSounds;

	//----------------------------------
	// ��Ÿ��

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

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bHitting;*/

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsParring;*/
};
