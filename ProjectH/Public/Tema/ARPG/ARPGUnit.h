// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/ARPGUnitBase.h"
#include "ARPGUnit.generated.h"


DECLARE_DELEGATE(FOnWeaponDraw)
DECLARE_DELEGATE(FOnWeaponStow)


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

	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamageCalculator(class AARPGWeapon* DamageWeapon, FDamageEvent const& DamageEvent, AController* EventInstigator, AARPGUnitBase* DamageCauser) override;

	// ������ ���Ƴ� AP�� ��ƾ��Ҷ� ����ϴ� �Լ�.
	virtual void TakeDamageAP(float Damage) override;
	//���� ���ߴ��� �Ǵ� �ϴ� �Լ�.
	virtual bool Hit(bool bBlockingHit) override;
	virtual void ParringHit(class AARPGUnitBase* InstigatorActor) override;

	virtual void ZeroAP() override; // AP�� �����̴�. AP ȸ���� ��� ����ߵ�

	// �׾����� ���� �ùķ���Ʈ Ȱ��ȭ
	virtual void DeathWeaponSimulate() override;
	// ����
	virtual void Death() override;
	
	//������ �������� �ݵ�� ȣ��Ǿ���� �Լ����� ��Ƶ� ��.
	virtual void EndAttack() override;

public:
	// ���� ���
	void PlaySound(class USoundBase* Sound);

	void ChargeAttackStart();
	void ChargeAttackEnd();
	void SetBossHPWidget(bool bFlag, class AARPGEnermy* Boss);
	bool IsBossHPWidget();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* FPSCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* DeathCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class USkeletalMeshComponent* FPSMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGPlayerController* OwnerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UARPG_UnitAnimInstance* FPSMeshAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UARPG_TPSAnimInstance* TPSMeshAnimInstance;

	FOnWeaponDraw OnWeaponDraw; // ���⸦ ������ �����ϴ� ��������Ʈ.
	FOnWeaponStow OnWeaponStow; // ���⸦ ��������� �����ϴ� ��������Ʈ.
	

	//------------------------------------------------------
		// ī�޶� ����ũ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_BattleMode_CS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_BlockingMode_CS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_BlockingZeroAPMode_CS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_PoiseHitMode_CS;

	// ī�޶� ����ũ ���� �Լ�
	void PlayCameraShake(TSubclassOf<class UCameraShakeBase> CS);

	//-----------------------------------------------------

	// L
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_Sword;
	/*UPROPERTY(BlueprintReadWrite)
		class AARPGWeapon* TPSWeapon;*/
	UPROPERTY(BlueprintReadWrite)
		class AARPGWeapon* FPSWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponSocketName;


	// R
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_Shield;
	/*UPROPERTY(BlueprintReadWrite)
		class AARPGWeapon* TPSShield;*/
	UPROPERTY(BlueprintReadWrite)
		class AARPGWeapon* FPSShield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ShieldSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName IdleSocketName;

	//---------------------------------------------------------------------

	void ResetMode(); // �������� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bNormalMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParringPlaying; // �̰� �и� �ִϸ��̼��� �����ϱ� ���� ����.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlockMode; // �̰� �ִϸ��̼��� ���� ����.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlocking; // �̰� ���� �� �ܰ����� �˻��ϴ� ����. �̰��� true���� ���� ���� �Ϸ� �ΰ�.

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

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float AttackCharge;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MouseSensivity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float WalkSpeed; // ���� �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlockSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LockOnRadius; // ���� �Ҽ� �ִ� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bTargeting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bChargeAttacking = false;


	//�� �������� ���� ������ ������ �ִ��� �Ǵ��ϴ� �Լ�.
	// ex) �и����� �и��Ҷ��� ���尡 Ǯ���µ�, 
	//		�ű⼭ ���� ���� RMB�� ������������ �и������� �˾Ƽ� BlockMode�� true�� �ȴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bLMBPush = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bRMBPush = false;

	//------------------------------------------------------

	UPROPERTY(BlueprintReadWrite)
		class APawn* TargetingPawn; // ���������� ã�� Ÿ���� ��

	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	UPROPERTY()
		TArray<AActor*> IgnoreActor;

	//--------------------------------------------------

	// LMB�� RMB�� �ϱ� ���� AP. ���⿡�� �����´�
	UPROPERTY(VisibleAnywhere)
		float LMB_AP;
	UPROPERTY(VisibleAnywhere)
		float RMB_AP;
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
		void Jumping();
	UFUNCTION()
		void StopJump();
	UFUNCTION()
		void Sprint();
	UFUNCTION()
		void SprintReleased();
	UFUNCTION()
		void Sheathed();
	UFUNCTION()
		void Parring();

	//------------------------------------------

	UFUNCTION()
		void StartUseAPFunction(); // AP�� ����ϴ� �׼��� ����ϸ� FOnUseAP ��������Ʈ�� ȣ�� �� �Լ�.
	UFUNCTION()
		void UsingAPFunction(); // AP�� ���������� ����ϴ� �Լ�.
	UFUNCTION()
		void EndAPFunction(); // AP�� ����ϴ� �׼��� ����ϸ� FOnEndAP ��������Ʈ�� ȣ�� �� �Լ�.

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
		void HitEnd();

	//------------------------------------------
	// ���� OFF
	void BlockingEnd();
	// ��ŷ �߿� �¾Ƽ� AP�� ���ΰ� ��.
	void ShieldZeroAP();
	// �и� ���� �� ����
	void SpecialAttack();

	void SetDeathCamera(); // �׾����� ī�޶� ĳ���� �Ӹ��� �޸� ������ ���� �Ǿ �������� ����.


public:
	void LockOn();
	void LockOnSetPosition(FVector TargetPos);
	void LockOnAddViewport(bool bFlag);


};
