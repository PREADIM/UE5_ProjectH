// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "ActorComponent/QuestComponent/NPCInterface.h"
#include "TriggerEventBase.generated.h"

UCLASS()
class PROJECTH_API ATriggerEventBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerEventBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString QuestName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString OwnerNPCName; // �� ����Ʈ�� OwnerNPC�̸�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString TargetNPCName; // �� ����Ʈ�� �����Ѿ��� TargetNPC�̸�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bTargetIsNPC; // ��ġ���־���ϴ� Ÿ���� NPC�ΰ�?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 QuestNumber; // �� ����Ʈ�� �ѹ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bOverlapHiddenWidget; // ������������ �Ÿ��� ��Ÿ���� ������ ���� ���ΰ�?
	// �ƴҽ� Destroy���� ����.

	//--------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBPBindHiddenTriggerFunc; // SetHiddenTriggerWidget�� ����ɶ� ���� ȣ��� �Լ��� �ִ°�?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBPBindVisibleTriggerFunc; // SetTriggerWidget�� ����ɶ� ���� ȣ��� �Լ��� �ִ°�?

	// �������Ʈ���� ���ε� �� �Լ�.
	UFUNCTION(BlueprintImplementableEvent)
		void SetHiddenTriggerWidgetBind();
	UFUNCTION(BlueprintImplementableEvent)
		void SetVisibleTriggerWidgetBind();

	//--------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UQuestComponent* QuestComponent;
	//����Ʈ ������Ʈ���� �����ϸ鼭 ����.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AProjectHCharacter* PlayerCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AProjectH_PC* PlayerController;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UProjectHGameInstance* GI;

	/*-------------------------
			����Ʈ ������
	---------------------------*/

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UQuestIcon> BP_QuestIcon;
	UPROPERTY()
		class UQuestIcon* QuestIcon;
	UPROPERTY()
		class UCanvasPanelSlot* IconCanvasSlot;

	/* �ش� Ʈ���ſ� �ڵ���������� ���̻� ����Ʈ �������� ��������ʱ⶧����, �ٽ� �������� �����Ҷ� 
		�������� ����� �־�� �ϴ°��� �Ǻ��ϱ� ���� �Ҹ��� ���� */
	UPROPERTY(EditAnywhere)
		bool bVisibleIcon = false;

	/*-------------------------------------*/

public:
	/*--------------------------------------------------------------------------
		�� �Լ��� ��Ʈ�ѷ��� MainIconWidget�� �ش� Ʈ������ ��ġ���� �˷��ִ� �Լ�.
	----------------------------------------------------------------------------*/
	void SetupMainIconWidget();

	/*---------------------------------------------------------------*/

	UFUNCTION()
		void OverlapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OverlapEndTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void SetupCollision(); // �ݸ��� ����Ű��.

	/* Ʈ���Ű� NPC�� ��ȯ�ؼ� �ش� NPC���� ����Ʈ�� �شٰų�,
	Ʈ���Ű� ���� �ؾ��ϴ� ���� ������ ����� �Լ�.
	���ӽ���� QuestComponent�� AddQuest���� �����Ų��. �������Ʈ���� �ۼ��ϵ��� �����.*/
	UFUNCTION(BlueprintImplementableEvent)
		void SetInit(); 

	UFUNCTION(BlueprintImplementableEvent)
		void TriggerDestroyBPBind(); // Ʈ���Ű� CompleteStep���� �����ɶ� ȣ�� �Ǿ��ϴ� �Լ�.
	// TriggerSpawnActor�� ��ȯ�Ǿ��ٸ� �ش� �Լ����� �����ش�.

	UFUNCTION(BlueprintCallable)
		void TriggerDestroy(); // �����Լ�.
	UFUNCTION(BlueprintCallable)
		void SetTriggerWidget(); // Ʈ������ ƽ�� Ȱ��ȭ�ϸ鼭 ���� �����ϱ�.
	UFUNCTION(BlueprintCallable)
		void SetHiddenTriggerWidget(); // �� �Լ��� �ݴ� ����. ���� ����.
	UFUNCTION(BlueprintCallable)
		bool IsThisTrigger();
	UFUNCTION(BlueprintCallable)
		void QuestNextStep();
	UFUNCTION(BlueprintCallable)
		void BindProgressCnt(); /* ������� �ִ� ����Ʈ�� ������� �ٽ� ���ε�. */
	UFUNCTION(BlueprintCallable)
		void QuestIconVisible(bool bFlag); /* ���� ����Ʈ ó�� Tick�� ���ΰ� ����Ʈ �����ܸ� ������� �ؾ��ϴ� ��츦 ���� �Լ�. */

	UFUNCTION(BlueprintCallable)
		void CanClearQuest(); // ����Ʈ�� Ŭ���������� NPC�� SucceedQuestsNums�� �ֽ�ȭ���ָ鼭, ĳ������ ����Ʈ �ݸ����� ���� ���ָ� �˾Ƽ� �ʱ�ȭ �ȴ�
};


