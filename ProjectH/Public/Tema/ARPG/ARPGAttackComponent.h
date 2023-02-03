// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Components/ActorComponent.h"
#include "Tema/ARPG/AttackClass.h"
#include "ARPGAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UARPGAttackComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UARPGAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintCallable)
		const TArray<class UAttackClass*>& GetAttacks() { return Attacks; }

	UFUNCTION(BlueprintCallable)
		class UAttackClass* GetAttackClass(int32 index);

	void Init(TArray<TSubclassOf<class UAttackClass>> BP_Attacks); // 설정 초기화 함수.

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class UAttackClass*> Attacks;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnitBase* OwnerUnit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 AttackCnt; // 어택이 등록된 수

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MaxAttackDistance; // 공격의 가장 최대 길이
};
