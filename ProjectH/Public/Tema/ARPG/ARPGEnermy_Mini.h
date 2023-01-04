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
	
	virtual void TakeDamageAP(float Damage) override;
	virtual void Hit() override;
	//공격 당했는지 판단 하는 함수.	
	virtual bool CanThisDamage() override;
	// 공격 할수 있는지 판단 하는 함수.
	virtual void ChangeBattleMode(bool bFlag) override;

	virtual void Attack(int32 index) override; // 공격 함수
	virtual void Guard(bool bFlag) override;
	virtual void Parring(bool bFlag) override;
	virtual void Death() override; 	// 죽음 함수

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

	void SetBlocking(bool bFlag); // BT에서 방패를 들건지 설정해줌
	void ZeroAP();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGWeapon* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_Shield;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGWeapon* Shield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ShieldSockName;

	//--------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlockMode; // 이건 애니메이션을 위한 변수.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlocking; // 이게 실제 블럭 단계인지 검사하는 변수. 이것이 true여야 쉴드 전개 완료 인것.
};
