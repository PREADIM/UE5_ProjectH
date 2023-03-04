// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Components/ActorComponent.h"
#include "QuestStruct.h"
#include "QuestStep.h"
#include "NPCQuest.h"
#include "QuestComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnUpdateQuestList); // 퀘스트를 클리어하거나 새로 추가할때
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateDescription, int32); // 퀘스트 완료 했을때


USTRUCT(BlueprintType)
struct FActiveQuest
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString QuestName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 QuestNumber;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString OwnerNPCName;

	FActiveQuest();
	void Clear();
	void SetActiveQuest(FQuestStruct& Quest);
};

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
	
	FOnUpdateQuestList OnUpdateQuestList; // 퀘스트리스트를업데이트 할때 실행할 델리게이트.
	FOnUpdateDescription OnUpdateDescription; // 퀘스트 한 단계 클리어시 호출할 델리게이트.

	UPROPERTY(VisibleAnywhere)
		class AProjectHCharacter* OwnerCharacter;

	UPROPERTY(VisibleAnywhere)
		class AProjectH_PC* OwnerController;

	UPROPERTY(VisibleAnywhere)
		class UProjectHGameInstance* GI;


public:
	/* ------------------
		public Function
	--------------------*/
	void AddQuest(FNPCQuest NPCQuest); // 퀘스트 UI에서 수락을 누르면 추가.

	void SelectQuest(FString SelectQuestName); // 퀘스트를 내가 골랐거나, 아예 고르지않았을때 퀘스트가 선택됨.
	// 퀘스트를 이미 한번이라도 수행했으면 퀘스트 못바꾸고, 아니면 바꿀수 있다.

	void RemoveQuest(int32 RemoveQuestNumber); // 완료한 퀘스트를 지운다.
	// 이 퀘스트 시스템은 반드시 추적하는 퀘스트만 완료되게 해놨지만 그런 것이 상관없다면 Remove역시 해당 퀘스트 인덱스를 받아와서 지운다..

	void CompleteStep(int32 QuestNumber); // QuestStep을 다음 단계로 넘긴다.

	UFUNCTION()
		void AcceptQuest(); // FOnAcceptEvent 델리게이트에 연동되어있는 함수.


	void UpdateQuestSave(); // 퀘스트가 변동되었을때 새로 세이브 파일에 저장하는 함수.

	void StartQuestLoad(); // 시작할때 퀘스트 로드하기.

	void BindSetDescription(int32 QuestIndex); // 몬스터나 횟수가 있는 퀘스트를 할때 현재 진행률 바인드

	void SaveQuestNumber(FString NPCName, int32 QuestNumber);

	void BeginSetupHaveQuests(); // 혹여나 퀘스트 중복 수락을 방지하기 위하여 HaveQuestNumber에 저장.

	void NewTriggerSet(int32 QuestIndex); // 퀘스트 트리거를 완료하면 그다음 트리거를 생성한다.

	/*-------------
		Get & Set
	--------------*/



	UFUNCTION(BlueprintCallable)
		FActiveQuest GetActiveQuest() { return ActiveQuest; }
	TArray<FQuestStruct> GetQuests() { return Quests; }
	int32 GetCurrentID() { return CurrentQuestID; }
	TSet<int32>* GetHaveQuestNums() { return &HaveQuestNumber; }

	void ActiveQuestClear() { ActiveQuest.Clear(); }

private:
	/* ----------------
		Private Value
	------------------*/

	UPROPERTY(VisibleAnywhere)
		FActiveQuest ActiveQuest; // 현재 실행중인 퀘스트.

	UPROPERTY(VisibleAnywhere)
		TArray<FQuestStruct> Quests; // 전체 가지고 있는 퀘스트 리스트.

	UPROPERTY(VisibleAnywhere)
		TSet<int32> HaveQuestNumber; // 가지고 있는 퀘스트번호를 검색하기 위한 것.

	UPROPERTY(VisibleAnywhere)
		  int32 CurrentQuestID = -1; // 현재 퀘스트의 번호


	/* ------------------
		Private Funtion
	--------------------*/

	void ResetQuest(); // 현재 진행하고있는 퀘스트와 퀘스트 아이디 초기화.
	void SelectTopQuest(int32 RemoveQuestNumber); // 진행하는 퀘스트가 없다면 자동으로 가장 상단의 퀘스트 실행.		
};
