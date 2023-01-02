// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Tema/ARPG/UnitState.h"
#include "ARPGUnitBase.generated.h"

UCLASS()
class PROJECTH_API AARPGUnitBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARPGUnitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:	
	/*----------------------------
	
			virtual function
	
	----------------------------*/
	virtual void Tick(float DeltaTime);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	virtual void ZeroAP() {}

public:

	//-----------공통 분모---------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FUnitState UnitState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentAttackFactor; // 현재 공격의 계수

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentAP_DMG; // 추가 AP 데미지

	//-------------------------------------------


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bZeroAP;
	// 이 변수는 방패나 스태미너를 다썻을때 true가 된다.


	//-------------------------------------------
	


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParring;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bHitting;
	
public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	float CalculDamage(float Damage);
	float CalculAPDamage(float APDamage);
};
