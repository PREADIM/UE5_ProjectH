// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "UObject/NoExportTypes.h"
#include "Tema/ARPG/ARPGAttackStruct.h"
#include "AttackClass.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackFunc);
DECLARE_MULTICAST_DELEGATE(FOnCppAttackFunc);

struct FSoundAndAttenuation;

USTRUCT(BlueprintType)
struct FAttackEffect
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect = nullptr;

	UPROPERTY(EditAnywhere)
		FName SocketName;
};


UCLASS(Blueprintable, BlueprintType)
class PROJECTH_API UAttackClass : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FAttackEffect> Effects;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 EffectCnt;

	// 캐릭터 목소리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSoundAndAttenuation> Sounds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 SoundCnt;

	// 공격 사운드 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSoundAndAttenuation> AttackSounds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 AttackSoundCnt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FARPGAttackStruct AttackStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnitBase* OwnerUnit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UWorld* World; // 타이머를 쓰기위한 월드. 캐릭터에서 반드시 받아와야함.
	// 해당 클래스는 월드에 스폰되지않는 실체가 없는 클래스 이기때문에 GetWorld가 불가능하다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAttackCoolTime; // 쿨타임이 존재하는 스킬인가? (온오프로 해두면 노쿨도 가능할듯)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CoolTime;
	UPROPERTY()
		float CurrentCoolTime = 0.f;

	UPROPERTY()
		FTimerHandle CoolTimeHandle;
	
public:
	void Init();
	void PlayAttack(class UWorld* UnitWorld); // Attack에서 몽타주가 실행되면 실행해야하는 함수.

	UFUNCTION()
		void CoolTimeStart(); // 쿨타임이 있는 스킬의 쿨타임 실행

	UFUNCTION()
		void CoolTimeFunc();

	UFUNCTION(BlueprintCallable)
		void SetCanThisAttack(bool bFlag);

	UFUNCTION(BlueprintImplementableEvent)
		void BindFunction();
	// 해당 클래스마다 실행해야할 함수가 있다면 이걸로 호출
};
