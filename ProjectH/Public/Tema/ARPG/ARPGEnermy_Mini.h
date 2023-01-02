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
	virtual void Attack(int32 index) override; // ���� �Լ�
	virtual void Garud(bool bFlag) override;
	virtual void Parring(bool bFlag) override;
	virtual void Death() override; 	// ���� �Լ�
	virtual void Hit(bool bFlag) override;
	//���� ���ߴ��� �Ǵ� �ϴ� �Լ�.
	
	virtual void ZeroAP() override;

	//---------------------------------------

	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	UFUNCTION(BlueprintCallable)
		void WeaponOverlapEnd();


	void PlayAttack(int32 index); // AttackComponent�ȿ� �ִ� TArray���� ���õ鿡 �ε����� ����
	// �ش� �ε����� ������ �ߵ��ǰ��ϴ� ���.


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
