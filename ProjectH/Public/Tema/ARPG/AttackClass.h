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

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite)
		FOnAttackFunc AttackFunc;
	//��� ����Ʈ�� ��������Ʈ

	FOnCppAttackFunc CppAttackFunc;
	//Cpp�� ��������Ʈ
	
public:
	//UAttackClass();
	void Init();
	void PlayAttack(); // Attack���� ��Ÿ�ְ� ����Ǹ� �����ؾ��ϴ� �Լ�.

	UFUNCTION(BlueprintCallable)
		void SetCanThisAttack(bool bFlag);
};
