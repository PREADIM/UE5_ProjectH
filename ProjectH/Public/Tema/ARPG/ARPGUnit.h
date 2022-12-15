// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "ARPGUnit.generated.h"

UCLASS()
class PROJECTH_API AARPGUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARPGUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGWeapon> BP_Sword;
	UPROPERTY(BlueprintReadWrite)
		class AARPGWeapon* Sword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AARPGShield> BP_Shield;
	UPROPERTY(BlueprintReadWrite)
		class AARPGShield* Shield;


	//---------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bNormalMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttackLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttackRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttackForward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAttackBackward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bShieldHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bParring;

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


	//------------------------------------------------------

	UPROPERTY(BlueprintReadWrite)
		class APawn* TargetingPawn; // 오버랩으로 찾은 타겟팅 폰

	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	UPROPERTY()
		TArray<AActor*> IgnoreActor;

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


	UFUNCTION(BlueprintImplementableEvent)
		void ParringMontage(int32 ParringState);
	// 1 앞 2 오 3 왼 4 뒤


public:
	void LockOn();
	void LockOnSetPosition(FVector TargetPos);


public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
