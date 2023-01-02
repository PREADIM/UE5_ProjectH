// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/ARPGUnitBase.h"
#include "ARPGUnit.generated.h"

UCLASS()
class PROJECTH_API AARPGUnit : public AARPGUnitBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARPGUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/*----------------------------
		virtual function
	----------------------------*/

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ZeroAP() override;
	// AP�� ���ΰ� �Ǹ� ����� �Լ�.

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* FPSCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class USkeletalMeshComponent* FPSMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UProjectHGameInstance* GI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGGameMode* GM;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGPlayerController* OwnerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UARPG_UnitAnimInstance* FPSMeshAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_CS;
	// ī�޶� ����ũ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_Sword;
	UPROPERTY(BlueprintReadWrite)
		class AARPGWeapon* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGShield> BP_Shield;
	UPROPERTY(BlueprintReadWrite)
		class AARPGShield* Shield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ShieldSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName IdleSocketName;

	//---------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bNormalMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttackLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttackRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttackForward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttackBackward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float AttackCharge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MouseSensivity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float WalkSpeed; // ���� �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalSpeed; // �⺻ �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BattleSpeed; // ��Ʋ �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlockSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LockOnRadius; // ���� �Ҽ� �ִ� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bTargeting = false;


	//------------------------------------------------------

	UPROPERTY(BlueprintReadWrite)
		class APawn* TargetingPawn; // ���������� ã�� Ÿ���� ��

	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	UPROPERTY()
		TArray<AActor*> IgnoreActor;

	//--------------------------------------------------

public:

	UFUNCTION()
		void Forward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void LookUp(float AxisValue);
	UFUNCTION()
		void LookRight(float AxisValue);

	UFUNCTION()
		void LMB();
	UFUNCTION()
		void LMBReleased();
	UFUNCTION()
		void RMB();
	UFUNCTION()
		void RMBReleased();

	UFUNCTION()
		void Sprint();
	UFUNCTION()
		void SprintReleased();
	UFUNCTION()
		void Sheathed();
	UFUNCTION()
		void Parring();


	//------------------------------------------

	UFUNCTION(BlueprintCallable)
		void SetWeaponCollision(bool bFlag);
	UFUNCTION(BlueprintCallable)
		void SetShieldCollision(bool bFlag);

	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	UFUNCTION(BlueprintCallable)
		void WeaponOverlapEnd();
	UFUNCTION(BlueprintCallable)
		void BlockEnd();

	//------------------------------------------

	void Death();
	void Hit(bool bFlag);
	//���� ���ߴ��� �Ǵ� �ϴ� �Լ�.


public:
	void LockOn();
	void LockOnSetPosition(FVector TargetPos);


public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
