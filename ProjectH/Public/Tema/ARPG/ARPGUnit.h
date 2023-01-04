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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 공격을 막아내 AP를 깎아야할때 사용하는 함수.
	virtual void TakeDamageAP(float Damage) override;
	//공격 당했는지 판단 하는 함수.
	virtual void Hit() override;
	virtual bool CanThisDamage() override;

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

	//------------------------------------------------------
		// 카메라 쉐이크
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_BattleMode_CS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_BlockingMode_CS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCameraShakeBase> BP_BlockingZeroAPMode_CS;

	// 카메라 쉐이크 실행 함수
	void PlayCameraShake(TSubclassOf<class UCameraShakeBase> CS);

	//-----------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_Sword;
	UPROPERTY(BlueprintReadWrite)
		class AARPGWeapon* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_Shield;
	UPROPERTY(BlueprintReadWrite)
		class AARPGWeapon* Shield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ShieldSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName IdleSocketName;

	//---------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bNormalMode;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float AttackCharge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MouseSensivity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float WalkSpeed; // 현재 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalSpeed; // 기본 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BattleSpeed; // 배틀 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlockSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LockOnRadius; // 락온 할수 있는 범위

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bTargeting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDeath = false;

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
		void HitEnd();

	void BlockingEnd();
	// 방패 OFF

	// AP가 제로이다.
	void ZeroAP();

	//------------------------------------------

	void Death();

public:
	void LockOn();
	void LockOnSetPosition(FVector TargetPos);
	void LockOnAddViewport(bool bFlag);


};
