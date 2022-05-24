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
	//퀘스트 컴포넌트에서 스폰하면서 대입.

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
	/* 트리거가 NPC를 소환해서 해당 NPC에게 퀘스트를 준다거나, 
	트리거가 무언가 해야하는 일이 있을때 사용할 함수.
	게임실행시 QuestComponent의 AddQuest에서 실행시킨다. 블루프린트에서 작성하도록 만든다.*/

	void SetTriggerWidget(); // 트리거의 틱을 활성화하면서 위젯 생성하기.
	void SetHiddenTriggerWidget(); // 위 함수의 반대 버전. 위젯 끄기.

	UFUNCTION(BlueprintCallable)
		void QuestClear();
	UFUNCTION(BlueprintCallable)
		bool IsThisTrigger();
	void QuestNextStep();
	
	UFUNCTION(BlueprintCallable)
		void BindProgressCnt(); /* 진행률이 있는 퀘스트의 진행률을 다시 바인드. */

};
