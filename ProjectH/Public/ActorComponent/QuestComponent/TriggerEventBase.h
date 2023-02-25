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
		FString OwnerNPCName; // 이 퀘스트의 OwnerNPC이름
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 QuestNumber; // 이 퀘스트의 넘버

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UQuestComponent* QuestComponent;
	//퀘스트 컴포넌트에서 스폰하면서 대입.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UWidgetComponent* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AProjectHCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectHGameInstance* GI;

public:
	/* 트리거가 NPC를 소환해서 해당 NPC에게 퀘스트를 준다거나,
	트리거가 무언가 해야하는 일이 있을때 사용할 함수.
	게임실행시 QuestComponent의 AddQuest에서 실행시킨다. 블루프린트에서 작성하도록 만든다.*/
	UFUNCTION(BlueprintImplementableEvent)
		void SetInit(); 
	UFUNCTION(BlueprintImplementableEvent)
		void TriggerDestroyBPBind(); // 트리거가 CompleteStep으로 삭제될때 호출 되야하는 함수.
	// TriggerSpawnActor가 소환되었다면 해당 함수에서 지워준다.

	void TriggerDestroy(); // 삭제함수.
	

	void SetTriggerWidget(); // 트리거의 틱을 활성화하면서 위젯 생성하기.
	void SetHiddenTriggerWidget(); // 위 함수의 반대 버전. 위젯 끄기.

	UFUNCTION(BlueprintCallable)
		bool IsThisTrigger();

	void QuestNextStep();
	
	UFUNCTION(BlueprintCallable)
		void BindProgressCnt(); /* 진행률이 있는 퀘스트의 진행률을 다시 바인드. */

	void ClearQuest(); // 퀘스트를 클리어했으니 NPC의 SucceedQuestsNums를 최신화해주면서, 캐릭터의 퀘스트 콜리전을 껏다 켜주면
	// 알아서 초기화 된다.

};
