// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "Tema/ARPG/ARPG_TPSAnimInstance.h"
#include "ARPG_UnitAnimInstance.generated.h"


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


	//----------------------------------
	// 노티파이

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


	//패링 성공 후 공격 시스템 ( 데미지 증가가 있어야함 )
	UFUNCTION()
		void AnimNotify_SpecialAttackEnd();

	UFUNCTION()
		void AnimNotify_Death();
	//----------------------------------


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnit* OwnerUnit;

	//----------------------------------
	// 이펙트 사운드 AttackClass에서 가져와서 노티파이에서 실행

	UPROPERTY(VisibleAnywhere)
		int32 CurrentEffectIndex;
	UPROPERTY(VisibleAnywhere)
		int32 CurrentSoundIndex;
	UPROPERTY(VisibleAnywhere)
		TArray<FAttackEffect> CurrentEffects;
	UPROPERTY(VisibleAnywhere)
		TArray<class USoundBase*> CurrentSounds;

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
