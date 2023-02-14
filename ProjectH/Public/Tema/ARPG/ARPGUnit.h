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

	// 공격을 막아내 AP를 깎아야할때 사용하는 함수.
	virtual void TakeDamageAP(float Damage) override;
	//공격 당했는지 판단 하는 함수.
	virtual bool Hit(bool bBlockingHit) override;
	virtual void ParringHit(class AARPGUnitBase* InstigatorActor) override;

	virtual void ZeroAP() override; // AP가 제로이다. AP 회복이 잠시 멈춰야됨

	// 죽었을때 무기 시뮬레이트 활성화
	virtual void DeathWeaponSimulate() override;
	// 죽음
	virtual void Death() override;
	
	//공격이 끝났을때 반드시 호출되어야할 함수들을 모아둔 것.
	virtual void EndAttack() override;

public:
	// 음악 재생
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

	FOnWeaponDraw OnWeaponDraw; // 무기를 뽑을때 실행하는 델리게이트.
	FOnWeaponStow OnWeaponStow; // 무기를 집어넣을때 실행하는 델리게이트.
	

	//------------------------------------------------------
		// 카메라 쉐이크
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_BattleMode_CS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_BlockingMode_CS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_BlockingZeroAPMode_CS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_PoiseHitMode_CS;

	// 카메라 쉐이크 실행 함수
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

	void ResetMode(); // 변수들을 끄기

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bNormalMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParringPlaying; // 이건 패링 애니메이션을 실행하기 위한 변수.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlockMode; // 이건 애니메이션을 위한 변수.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlocking; // 이게 실제 블럭 단계인지 검사하는 변수. 이것이 true여야 쉴드 전개 완료 인것.

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
		float WalkSpeed; // 현재 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlockSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LockOnRadius; // 락온 할수 있는 범위

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bTargeting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bChargeAttacking = false;


	//이 변수들은 내가 여전히 누르고 있는지 판단하는 함수.
	// ex) 패링에서 패링할때는 가드가 풀리는데, 
	//		거기서 내가 아직 RMB를 누르고있으면 패링끝나고 알아서 BlockMode가 true가 된다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bLMBPush = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bRMBPush = false;

	//------------------------------------------------------

	UPROPERTY(BlueprintReadWrite)
		class APawn* TargetingPawn; // 오버랩으로 찾은 타겟팅 폰

	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	UPROPERTY()
		TArray<AActor*> IgnoreActor;

	//--------------------------------------------------

	// LMB와 RMB를 하기 위한 AP. 무기에서 가져온다
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
		void StartUseAPFunction(); // AP를 사용하는 액션을 사용하면 FOnUseAP 델리게이트로 호출 될 함수.
	UFUNCTION()
		void UsingAPFunction(); // AP를 지속적으로 사용하는 함수.
	UFUNCTION()
		void EndAPFunction(); // AP를 사용하는 액션을 사용하면 FOnEndAP 델리게이트로 호출 될 함수.

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
	// 방패 OFF
	void BlockingEnd();
	// 블럭킹 중에 맞아서 AP가 제로가 됨.
	void ShieldZeroAP();
	// 패링 성공 후 공격
	void SpecialAttack();

	void SetDeathCamera(); // 죽었을때 카메라가 캐릭터 머리에 달린 것으로 블렌드 되어서 역동적인 연출.


public:
	void LockOn();
	void LockOnSetPosition(FVector TargetPos);
	void LockOnAddViewport(bool bFlag);


};
