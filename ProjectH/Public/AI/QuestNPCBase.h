// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Pawn.h"
#include "NPCAllQuest.h"
#include "Dialogue/TextNAnim.h"
#include "ActorComponent/QuestComponent/NPCInterface.h"
#include "QuestNPCBase.generated.h"


USTRUCT(BlueprintType)
struct FNormalDialogue
{
	GENERATED_USTRUCT_BODY()
public:
	/*UPROPERTY(EditAnywhere)
		TArray<FText> Dialogue;*/

	UPROPERTY(EditAnywhere)
		TArray<FTextNName> Dialogue;

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
	//virtual void Tick(float DeltaTime) override;


public:
	/*-------------------
		public Function
	---------------------*/

	void SetNPCQuests(FNPCAllQuest& LoadQuests) { NPCQuests = LoadQuests; }


public:
	/*-----------------
		Public Value
	-----------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FNPCAllQuest NPCQuests; // 가지고 있을 퀘스트 목록들.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UCapsuleComponent* CapsuleCollision;

	/*-----------------------
		Quest Widget Value
	------------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UWidgetComponent* IconWidget; // 그냥 퀘스트 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UWidgetComponent* MainIconWidget; // 메인 퀘스트 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UWidgetComponent* SucceedWidget; // 완료 아이콘

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
		FString NPCName; // NPC 이름. 데이터 베이스 검색이나 저장할때 용이하다.
	UPROPERTY(VisibleAnywhere)
		class UProjectHGameInstance* GI;


	/*--------------------
		Quest Property
	--------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bQuestSucceed; // 퀘스트 완료인가?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsQuesting; // 퀘스트 중인가?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanAccept; // 캐릭터가 퀘스트를 수락가능한 NPC인가? (위젯 띄우기용)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 CanQuestCnt; // 얼마나 수락가능한 퀘스트가 많은가?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 CanMainQuestCnt; // 얼마나 수락가능한 퀘스트가 많은가?

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bHaveMainQuest; // 메인 퀘스트를 가지고있는가? 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AProjectHCharacter* PlayerCharacter;


	/*--------------------
		Dialogue Property
	--------------------*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FNormalDialogue> NormalDialogue;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FNormalDialogue NormalDialogue;


public:
	/*-------------------
		Public Function
	--------------------*/

	void AddNPCQuest(FNPCQuest Quest, bool bSucceed); // NPC 퀘스트 만들어서 실시간으로 전달하기 함수.

	virtual void Interact_Implementation(class AProjectHCharacter* OwnerCharacter);

	void QuestInfoOpen(int32 QuestIndex, class AProjectH_PC* OwnerController);
	void SaveNPCQuest(); // 게임 인스턴스 안에있는 세이브 클래스에 저장하는 함수.

	void SetIconWidget();
	void HiddenIcon();

	bool FindCanQuest(); // 캐릭터가 해당 NPC의 퀘스트를 수행조건이 가능한지 검사한다.

	bool CanVisibleWidget(); 
	// 위젯을 띄워도 되는지 판단한다. 캐릭터클래스에서 NPC의 위젯을 띄워야할때 판단하는 함수.

};
