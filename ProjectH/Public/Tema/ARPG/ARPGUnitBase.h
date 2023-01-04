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
	// TakeDamage의 AP버전
	virtual void TakeDamageAP(float Damage) {}

	// 해당 함수는 모든 클래스들이 따로 제작해야한다.
	// 맞았을때 이 함수를 공통적으로 실행하고 각 유닛의 모드별로 알아서 애님인스턴스에 해당하는 모드 히트를
	// 알려주는 형식
	virtual void Hit() {}

	// 지금 이 유닛에 데미지를 줄수 있는가?
	virtual bool CanThisDamage() { return false; }
	
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
		bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParring;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bHitting;
	
public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	float CalculDamage(float Damage);
	float CalculAPDamage(float APDamage);
};
