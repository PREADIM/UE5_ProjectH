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
	AProjectHCharacter();


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
		void EndPlaySequence(); // ���� �����Ҷ� �� ó�� �������� ����ɶ� CollisionSetup�� 0.1�� �ڿ� �ٷ� ����Ǿ �����̺��δ�.
	// ������ ������ ������ Setup ����.
	// �� ������ ������ �� ���͸��� �������ֱ⶧���� �߻��� ���̴�.
	// ���ʿ� ���� �������� ���Ͱ��� �Ÿ��� ��Ÿ�����ٸ� ���������� ���� �˾Ƽ� �������� ���ϴ�.

	UFUNCTION(BlueprintImplementableEvent)
		void BeginPlayBPBind(); // �ڡ�BP���� �߰��� ����

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
	

	/* ���� �Է� */
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
	/*--------------------
		Public Function
	---------------------*/

	UQuestComponent* GetQuestComponent() { return QuestComponent; }
	AProjectH_PC* GetOwnerController() { return OwnerController; }

	UFUNCTION()
		void QuestCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void QuestCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void InteractCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void InteractCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void QuestCollisionSetUp(); /* ����Ʈ �ݸ����� ũ�⸦ �ٿ��ٰ� �ٽ� �÷��� ���� ������ �ǰ��ϴ� Ʈ��*/
	void InteractCollisionSetUp();

	UFUNCTION()
		void QuestCollisionRestart();
	UFUNCTION()
		void InteractCollisionRestart();

	/*---------------------
		virtual Function
	---------------------*/
	virtual void Tick(float DeltaTime);
	virtual void OverlapedEvent_Implementation() {}
	virtual void PostInitializeComponents();
};

