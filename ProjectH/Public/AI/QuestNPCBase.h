// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Pawn.h"
#include "QuestDataBase.h"
#include "Dialogue/TextNAnim.h"
#include "ActorComponent/QuestComponent/NPCInterface.h"
#include "QuestNPCBase.generated.h"


USTRUCT(BlueprintType)
struct FNormalDialogue
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		TArray<FTextNName> Dialogue;
};

USTRUCT()
struct FNPCQuestPTR
{
	GENERATED_USTRUCT_BODY()
public:
	FNPCQuest* NPCQuestPTR;
};


// 퀘스트의 상태에따라 위젯을 어떻게 띄울지 나타낼 상태 열거형
UENUM()
enum class EQuestIconState : uint8
{
	NONE UMETA(DisplayName = "NONE"),

	SubCanQuest UMETA(DisplayName = "SubCanQuest"),
	MainCanQuest UMETA(DisplayName = "MainCanQuest"),

	SubQuesting UMETA(DisplayName = "SubQuesting"),
	MainQuesting UMETA(DisplayName = "MainQuesting"),

	SubSucceedQuest UMETA(DisplayName = "SubSucceedQuest"),
	MainSucceedQuest UMETA(DisplayName = "MainSucceedQuest")
};


UCLASS()
class PROJECTH_API AQuestNPCBase : public APawn, public INPCInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AQuestNPCBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	/*-----------------
		Public Value
	-----------------*/
	/*
	* 퀘스트는 기본적으로 수락가능 -> 진행중 -> 완료가능 -> 완료 순서이다.
	* 
	NPCQuests 자체는 완료던 진행중이던 일단 NPC의 모든 퀘스트를 가지고있는다.
	QuestingNums와 SucceedQuestsNums는 각각 퀘스트'중'인 넘버와 완료가 '가능'한 퀘스트들의 넘버이다.
	해당 번호들을 NPCQuestPtrs에 넘버로 검색해서 해당 플래그들을 켜준다.

	퀘스트를 완료 가능해지면 SucceedQuestsNums에 번호를 추가하고 QuestingNums에서 번호를 지운다.
	그리고 퀘스트 완료 후시 EndedQuestsNums에 번호를 추가하고 SucceedQuestsNums에서 번호를 지운다.
	★(QuestingNums -> SucceedQuestsNums -> EndedQuestsNums가 추가되면서 지워지는 순서)
	*/


	/*-----------------------
			Component
	------------------------*/

	UPROPERTY(EditAnywhere, Category = Component)
		class USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category = Component)
		class USceneComponent* IconLocationComponent;

	/*------------------------------------------*/

	UPROPERTY(VisibleAnywhere)
		EQuestIconState QuestIconState;
	/**/
	int32 RetIconPriority(EQuestIconState State); // 각 위젯의 상태에 가중치를 설정해 계산해서 우선도를 판단한다.
	void SetQuestIconState(EQuestIconState NewState); // QuestIconState의 가중치를 판단해서 계산하여 값 설정.
	/**/


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FNPCAllQuest NPCQuests; // 가지고 있을 퀘스트 목록들.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSet<int32> QuestingNums; // 퀘스트 중인 넘버들 (세이브 로드)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSet<int32> SucceedQuestsNums; // 완료가능 퀘스트 넘버들. (세이브 로드)
	// 완료가능'한' 퀘스트가 아니라 완료 퀘스트이다.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSet<int32> EndedQuestsNums; // 완료'한' 퀘스트 넘버들. (세이브 로드)

	UPROPERTY(VisibleAnywhere)
		TMap<int32, FNPCQuestPTR> NPCQuestPtrs; // 퀘스트 넘버로 해당 퀘스트에 접근 할수 있는 주소 모음집.
	// 해당 맵을 통해 완료한 퀘스트에 접근해서 완료트리거를 설정해준다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
		FString NPCName; // NPC 이름. 데이터 베이스 검색이나 저장할때 용이하다.

	/*-----------------------
		Quest Icon Widget
	------------------------*/

	UPROPERTY(EditAnywhere, Category = Widget)
		TSubclassOf<class UNormalIconUI> BP_NPCQuestIconUI;
	UPROPERTY()
		class UNormalIconUI* NPCQuestIconUI;
	UPROPERTY()
		class UCanvasPanelSlot* NPCIconSlot;

	/*---------------------------------------------------*/

	UPROPERTY(VisibleAnywhere)
		class UProjectHGameInstance* GI;


	/*--------------------
		Quest Property
	--------------------*/

	//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AProjectH_PC* PlayerController;

	/*--------------------
		Dialogue Property
	--------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FNormalDialogue NormalDialogue;

public:
	/*-------------------
		Public Function
	--------------------*/

	virtual void Interact_Implementation(class AProjectHCharacter* OwnerCharacter);

	void QuestInfoOpen(int32 QuestIndex, class AProjectH_PC* OwnerController);
	
	void QuestSucceedInfoOpen(int32 QuestIndex, class AProjectH_PC* OwnerController);

	void NPCQuestSetup(); // 한번에 반드시 실행되어야 할 아래 중요 함수들을 모아둔것.
	//FindCanQuest, SetIconWidget, SaveNPCQuest

	/* 핵심 함수. */ 
	bool FindCanQuest(); // 캐릭터가 해당 NPC의 퀘스트를 수행조건이 가능한지 검사한다.
	void SaveNPCQuest(); // 게임 인스턴스 안에있는 세이브 클래스에 저장하는 함수.
	void SetIconWidget(); // 아이콘 출력.
	void HiddenIcon(); // 아이콘 가리기.
};
