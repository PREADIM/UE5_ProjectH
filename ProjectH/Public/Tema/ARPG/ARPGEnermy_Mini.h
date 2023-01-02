// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "ARPGEnermy_Mini.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AARPGEnermy_Mini : public AARPGEnermy
{
	GENERATED_BODY()

public:
	AARPGEnermy_Mini();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void Attack(int32 index) override; // 공격 함수
	virtual void Garud(bool bFlag) override;
	virtual void Parring(bool bFlag) override;
	virtual void Death() override; 	// 죽음 함수
	virtual void Hit(bool bFlag) override;
	//공격 당했는지 판단 하는 함수.
	
	virtual void ZeroAP() override;

	//---------------------------------------

	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	UFUNCTION(BlueprintCallable)
		void WeaponOverlapEnd();


	void PlayAttack(int32 index); // AttackComponent안에 있는 TArray형의 어택들에 인덱스를 보내
	// 해당 인덱스의 어택이 발동되게하는 방식.


	UFUNCTION(BlueprintCallable)
		void SetWeaponCollision(bool bFlag);

	UFUNCTION(BlueprintCallable)
		void SetShieldCollision(bool bFlag);

	//-----------------------------------------


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGWeapon* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGShield> BP_Shield;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGShield* Shield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ShieldSockName;

	//--------------------------------------------------

};
