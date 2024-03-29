// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "Interface/OverlapActorInterface.h"
#include "PhysicalSoundStruct.h"
#include "ProjectHCharacter.generated.h"


/*----------------------
	Character Template
-----------------------*/

UCLASS()
class AProjectHCharacter : public ACharacter, public IOverlapActorInterface
{
	GENERATED_BODY()


public:
	AProjectHCharacter();

	/*---------------------
		virtual Function
	---------------------*/
	virtual void Tick(float DeltaTime);
	virtual void OverlapedEvent_Implementation() {}
	virtual void PostInitializeComponents();

protected:
	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);


protected:
	/* --------------
		Component
	----------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		class USphereComponent* QuestCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		class UBoxComponent* InteractCollision;

	UPROPERTY(EditAnywhere, Category = Collision)
		FVector InteractBoxSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UQuestComponent* QuestComponent;

	UPROPERTY(VisibleAnywhere)
		class AProjectH_PC* OwnerController;

	UPROPERTY(VisibleAnywhere)
		class UProjectHAnimInstance* AnimInstance;

	//-------------------------------------------------------------

	UFUNCTION()
		void EndPlaySequence(); // 게임 시작할때 맨 처음 시퀀스가 실행될때 CollisionSetup이 0.1초 뒤에 바로 실행되어서 위젯이보인다.

	UFUNCTION(BlueprintImplementableEvent)
		void BeginPlayBPBind(); // BP에서 추가로 실행

public:
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
	
	/* 공통 입력 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);


public:
	/* ----------------
		Public Value
	------------------*/

	UPROPERTY(VisibleAnywhere)
		float MouseSensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RunSpeed)
		float RunningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RunSpeed)
		float WalkSpeed;

	UPROPERTY(VisibleAnywhere)
		bool bCanInteract = false;


	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	UPROPERTY()
		TArray<AActor*> IgnoreActor;


	UPROPERTY()
		class AQuestNPCBase* InteractNPCActor;
	UPROPERTY()
		class ATriggerSpawnActor* InteractActor;

	bool bQTE = false;
	bool bPlay = true;
public:
	/*----------
		Init
	------------*/

	void QuestComponentInit();

	/*----------
		Quest
	------------*/

	UQuestComponent* GetQuestComponent() { return QuestComponent; }
	AProjectH_PC* GetOwnerController() { return OwnerController; }
	void FromOtherMapNextQuestStep(int32 NextStepNumber);

	UFUNCTION()
		void QuestCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void QuestCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void QuestCollisionSetUp(); /* 퀘스트 콜리전을 껏다 켜서 새로 오버랩 되게하는 트릭*/
	UFUNCTION()
		void QuestCollisionRestart();

	/*---------------
		Interaction
	----------------*/

	UFUNCTION()
		void InteractCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void InteractCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void InteractCollisionSetUp();
	UFUNCTION()
		void InteractCollisionRestart();

	/*-------------------------
		피지컬 머터리얼 사운드
	---------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EPhysicalSurface>, FPhysicalSoundStruct> PhysicalAllSounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FPhysicalSoundStruct PhysicalSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SurfaceDistance = 150.f; // 땅끝의 거리

	// 사운드를 해당 표면에 맞춰서 설정해두고 애님블프에서 플레이 하도록 한다.
	void SetPhysicalSound();
};

