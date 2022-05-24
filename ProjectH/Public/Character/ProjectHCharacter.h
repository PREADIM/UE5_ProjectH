// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Interface/OverlapActorInterface.h"
#include "ProjectHCharacter.generated.h"


/*----------------------
	Character Template
-----------------------*/

UCLASS()
class AProjectHCharacter : public ACharacter, public IOverlapActorInterface
{
	GENERATED_BODY()

public:	
	/*--------------------
		TPS & FPS Change
	---------------------*/
	enum class ControllerView
	{
		FIRST,
		THRID
	};

	float MinSpringArmLength = 10.f; // 자연스러운 시점 변환할때 최소 길이. 수정 불가.
	bool bChangeView = false;

	ControllerView CurrentView = ControllerView::THRID;
	bool bTPS() 
	{ 
		if (CurrentView == ControllerView::THRID)
			return true;
		else
			return false;
	}

public:
	AProjectHCharacter();


protected:
	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

protected:
	/* --------------
		Component
	----------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class USphereComponent* QuestCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh1P, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh1P;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SpringArm, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera3P, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera3P;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UInvisibleWallComponent* InvisibleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UUtilityComponent* UtilityComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UQuestComponent* QuestComponent;

	UPROPERTY(VisibleAnywhere)
		class AProjectH_PC* OwnerController;

	UPROPERTY(VisibleAnywhere)
		class UProjectHAnimInstance* AnimInstance;

protected:
	/*----------------
		Move & Utils
	-----------------*/
	UFUNCTION()
		void Forward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void LookUp(float AxisValue);
	UFUNCTION()
		void LookRight(float AxisValue);

	UFUNCTION()
		void InteractKey();
	UFUNCTION()
		void AnyKey(FKey Key);
	UFUNCTION()
		void Run();
	UFUNCTION()
		void NotRun();
	UFUNCTION()
		void WheelDown();
	UFUNCTION()
		void WheelUp();


	virtual void LMB() {};
	virtual void RMB() {};



	/* 공통 입력 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);


public:
	/* ----------------
		Public Value
	------------------*/
	UPROPERTY()
		float SpringArmLength;
	UPROPERTY()
		FVector SpringArmLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpingArm)
		float MaxArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpingArm)
		float MinArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpingArm)
		FVector MaxArmLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpingArm)
		FVector MinArmLocation;
	/* 여기까지 스프링 암. */


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaxSpingArm)
	//	float MaxSpringArmLength; // 자연스러운 시점 변환할때 최대 길이 및 최대 카메라 거리.


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RunSpeed)
		float RunningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RunSpeed)
		float WalkSpeed;


	bool bCanInteract = false;
	bool bWheel = false;
	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> Object;
	UPROPERTY()
		TArray<AActor*> IgnoreActor;

	UPROPERTY()
		TArray<AActor*> NPCActors;

	bool bQTE = false;
	bool bPlay = true;
public:
	/*--------------------
		Public Function
	---------------------*/
	//void SetViewChange();
	//void SetControllerMode(ControllerView CView);

	UQuestComponent* GetQuestComponent() { return QuestComponent; }
	UFUNCTION()
		void QuestCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void QuestCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//UFUNCTION(BlueprintCallable) 일단 보류
		void QuestCollisionSetUp(); /* 퀘스트 콜리전의 크기를 줄였다가 다시 늘려서 새로 오버랩 되게하는 트릭*/
	/*---------------------
		virtual Function
	---------------------*/
	virtual void Tick(float DeltaTime);
	virtual void OverlapedEvent_Implementation() {}
	virtual void PostInitializeComponents();
};

