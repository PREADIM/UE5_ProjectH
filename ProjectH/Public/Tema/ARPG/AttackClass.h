// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "UObject/NoExportTypes.h"
#include "Tema/ARPG/ARPGAttackStruct.h"
#include "AttackClass.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackFunc)
DECLARE_MULTICAST_DELEGATE(FOnCppAttackFunc)

UCLASS(Blueprintable, BlueprintType)
class PROJECTH_API UAttackClass : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FARPGAttackStruct AttackStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnitBase* OwnerUnit;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite)
		FOnAttackFunc AttackFunc;
	//블루 프린트용 델리게이트

	FOnCppAttackFunc CppAttackFunc;
	//Cpp용 델리게이트
	
public:
	//UAttackClass();
	void Init();
	void PlayAttack(); // Attack에서 몽타주가 실행되면 실행해야하는 함수.

	UFUNCTION(BlueprintCallable)
		void SetCanThisAttack(bool bFlag);

	UFUNCTION(BlueprintImplementableEvent)
		void BindFunction();
	// 해당 클래스마다 실행해야할 함수가 있다면 이걸로 호출
};
