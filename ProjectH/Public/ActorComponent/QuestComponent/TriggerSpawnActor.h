// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "ActorComponent/QuestComponent/NPCInterface.h"
#include "TriggerSpawnActor.generated.h"

UCLASS()
class PROJECTH_API ATriggerSpawnActor : public AActor, public INPCInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerSpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TriggerText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AProjectHCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UQuestComponent* QuestComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class ATriggerEventBase* OwnerTrigger; // 이 액터가 트리거에 의해 실행된다면,
	// 이 액터의 할일을 다했을시에 Trigger의 QuestNextStep()을 실행해준다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBPBindFunc = true; // BPBindFunc를 사용한다면 이걸 True로 해준다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanThisInteract; // 이 액터가 현재 인터랙트 가능한지 여부.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bFlag; // 기본적인 On Off를 할때 사용할 변수. 이게 부모에 가지고있어야 편하게 해당 Flag를 보고 Trigger를 Dstroy할수있음.

public:
	virtual void Interact_Implementation(class AProjectHCharacter* OwnerCharacter);

	// ★★ Trigger에서 스폰된다면 스폰후 반드시 실행
	UFUNCTION(BlueprintCallable)
		void SetTriggerBaseInit(class ATriggerEventBase* TriggerBase, AProjectHCharacter* OwnerCharacter);

	// 트리거에 의한 스폰된 액터일시 퀘스트 완료시 오너 트리거에게 퀘스트 완료를 호출,
	UFUNCTION(BlueprintCallable)
		void ClearQuest();

	UFUNCTION(BlueprintImplementableEvent)
		void BPBindFunc();
};
