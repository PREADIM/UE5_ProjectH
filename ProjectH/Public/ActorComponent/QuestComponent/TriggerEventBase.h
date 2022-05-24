// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UQuestComponent* QuestComponent;
	//����Ʈ ������Ʈ���� �����ϸ鼭 ����.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UWidgetComponent* Widget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class AProjectHCharacter* PlayerCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsQuesting; // ?


public:
	UFUNCTION(BlueprintImplementableEvent)
		void SetInit(); 
	/* Ʈ���Ű� NPC�� ��ȯ�ؼ� �ش� NPC���� ����Ʈ�� �شٰų�, 
	Ʈ���Ű� ���� �ؾ��ϴ� ���� ������ ����� �Լ�.
	���ӽ���� QuestComponent�� AddQuest���� �����Ų��. �������Ʈ���� �ۼ��ϵ��� �����.*/

	void SetTriggerWidget(); // Ʈ������ ƽ�� Ȱ��ȭ�ϸ鼭 ���� �����ϱ�.
	void SetHiddenTriggerWidget(); // �� �Լ��� �ݴ� ����. ���� ����.

	UFUNCTION(BlueprintCallable)
		void QuestClear();
	UFUNCTION(BlueprintCallable)
		bool IsThisTrigger();
	void QuestNextStep();
	
	UFUNCTION(BlueprintCallable)
		void BindProgressCnt(); /* ������� �ִ� ����Ʈ�� ������� �ٽ� ���ε�. */

};
