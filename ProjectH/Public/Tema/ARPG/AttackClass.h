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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UWorld* World; // Ÿ�̸Ӹ� �������� ����. ĳ���Ϳ��� �ݵ�� �޾ƿ;���.
	// �ش� Ŭ������ ���忡 ���������ʴ� ��ü�� ���� Ŭ���� �̱⶧���� GetWorld�� �Ұ����ϴ�.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCoolTime; // ��Ÿ���� �����ϴ� ��ų�ΰ�? (�¿����� �صθ� ���� �����ҵ�)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CoolTime;
	float CurrentCoolTime = 0.f;
	FTimerHandle CoolTimeHandle;

	FOnCppAttackFunc CppAttackFunc;
	//Cpp�� ��������Ʈ
	
public:
	void Init();
	void PlayAttack(class UWorld* UnitWorld); // Attack���� ��Ÿ�ְ� ����Ǹ� �����ؾ��ϴ� �Լ�.

	UFUNCTION()
		void CoolTimeStart(); // ��Ÿ���� �ִ� ��ų�� ��Ÿ�� ����

	UFUNCTION()
		void CoolTimeFunc();

	UFUNCTION(BlueprintCallable)
		void SetCanThisAttack(bool bFlag);

	UFUNCTION(BlueprintImplementableEvent)
		void BindFunction();
	// �ش� Ŭ�������� �����ؾ��� �Լ��� �ִٸ� �̰ɷ� ȣ��
};
