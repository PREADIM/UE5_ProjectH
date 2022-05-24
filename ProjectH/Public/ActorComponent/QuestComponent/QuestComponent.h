// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Components/ActorComponent.h"
#include "QuestStruct.h"
#include "QuestStep.h"
#include "NPCQuest.h"
#include "QuestComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAddedRemovedQuest); // 퀘스트를 클리어하거나 새로 추가할때
DECLARE_MULTICAST_DELEGATE(FOnActiveQuestChange); // 진행할 퀘스트를 변경했을때
DECLARE_MULTICAST_DELEGATE(FOnAcceptEvent); 
DECLARE_MULTICAST_DELEGATE(FOnCompleteStep); // 퀘스트 완료 했을때
DECLARE_MULTICAST_DELEGATE(FOnSetDescription); // 횟수가 있는 퀘스트를 할때 진행률 변화.


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	/* ------------------
		public Value
	--------------------*/
	
	FOnAddedRemovedQuest OnAddedRemovedQuest; // 퀘스트가 추가 및 삭제 되었을때 실행할 델리게이트.
	FOnActiveQuestChange ActiveQuestChange; // 액티브 퀘스트가 변경되었을때 실행할 델리게이트
	FOnAcceptEvent AcceptEvent; // 퀘스트 수락할 시에 호출될 함수들
	FOnCompleteStep CompleteQuestDelegate; // 퀘스트 한 단계 클리어시 호출할 델리게이트.
	FOnSetDescription SetDescriptionDelegate; // 횟수가 있는 퀘스트를 할때 진행률 변화.

	/* 수락한 퀘스트의 NPC를 가져와서 NPC의 Index째의 퀘스트를 지운다.
		또는 퀘스트 트리거에 해당 NPC의 정보를 넘겨준다.*/
	UPROPERTY()
		class AQuestNPCBase* NewNPC;
	int32 NewNPCQuestIndex;


public:
	/* ------------------
		public Function
	--------------------*/
	void AddQuest(FNPCQuest NPCQuest); // 퀘스트 UI에서 수락을 누르면 추가.

	void SelectQuest(FString SelectQuestName); // 퀘스트를 내가 골랐거나, 아예 고르지않았을때 퀘스트가 선택됨.
	// 퀘스트를 이미 한번이라도 수행했으면 퀘스트 못바꾸고, 아니면 바꿀수 있다.

	void RemoveQuest(); // 완료한 퀘스트를 지운다.

	void CompleteStep(); // QuestStep을 다음 단계로 넘긴다.

	UFUNCTION()
		void UpdateQuestCashes(); // 퀘스트 쉽게 찾을 수 있도록 캐쉬 설정.
	UFUNCTION()
		void AcceptQuest(); // FOnAcceptEvent 델리게이트에 연동되어있는 함수.


	void UpdateQuestSave(); // 퀘스트가 변동되었을때 새로 세이브 파일에 저장하는 함수.

	void StartQuestLoad(); // 시작할때 퀘스트 로드하기.

	void BindSetDescription(); // 몬스터나 횟수가 있는 퀘스트를 할때 현재 진행률 바인드

	void FinishedQuestNumber(int32 QuestNumber); // 퀘스트 완료하면 변경할 함수.
	void SaveQuestNumber(int32 QuestNumber);
	// NPC 이름까지 보내서 인스턴스 안에있는 TMap까지 제거 해도될듯 하다. ★★
	// 굳이 이게임에서는 필요없으니 성능을 위해 구현하지 않음.




	/*-------------
		Get & Set
	--------------*/

	FQuestStruct GetActiveQuest() { return ActiveQuest; }
	TArray<FQuestStruct> GetQuests() { return Quests; }
	TMap<FString, FQuestStruct> GetQuestCashes() { return QuestCashes; }
	int32 GetCurrentID() { return CurrentQuestID; }





private:
	/* ----------------
		Private Value
	------------------*/
	UPROPERTY(VisibleAnywhere)
		FQuestStruct ActiveQuest; // 현재 실행중인 퀘스트.

	UPROPERTY(VisibleAnywhere)
		TArray<FQuestStruct> Quests; // 전체 가지고 있는 퀘스트 리스트.

	UPROPERTY(VisibleAnywhere)
		TMap<FString, FQuestStruct> QuestCashes;

	UPROPERTY(VisibleAnywhere)
		  int32 CurrentQuestID = -1; // 현재 퀘스트의 번호

	UPROPERTY(VisibleAnywhere)
		class AProjectHCharacter* OwnerCharacter;

	UPROPERTY(VisibleAnywhere)
		class UProjectHGameInstance* GI;

	/* ------------------
		Private Funtion
	--------------------*/

	void ResetQuest(); // 현재 진행하고있는 퀘스트와 퀘스트 아이디 초기화.
	void SelectTopQuest(); // 진행하는 퀘스트가 없다면 자동으로 가장 상단의 퀘스트 실행.


		
};
