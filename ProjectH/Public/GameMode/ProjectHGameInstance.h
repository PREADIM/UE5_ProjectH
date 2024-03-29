// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Engine/GameInstance.h"
#include "QuestDataBase.h"
#include "Dialogue/TextNAnim.h"
#include "Dialogue/DialogueStruct.h"
#include "Engine/DataTable.h"
#include "UObject/UObjectGlobals.h"
#include "ProjectHGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCanQuestNums
{
	GENERATED_USTRUCT_BODY()
public:
	TSet<int32> QuestNums;

	FCanQuestNums();
	FCanQuestNums(int32 QuestNumber);

};

USTRUCT(BlueprintType)
struct FLevelPath : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		FString LevelName;
	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<class UWorld> Level;
};


USTRUCT(BlueprintType)
struct FSequenceActorTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		int32 SequenceNumber = 0;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class APlaySequenceActor> BP_SequenceActor;
	
	UPROPERTY(EditAnywhere)
		FString Description;

	FSequenceActorTable()
	{
	}
};


UCLASS()
class PROJECTH_API UProjectHGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UProjectHGameInstance();
	virtual void Init() override;

	UPROPERTY()
		class ULevelSequencePlayer* SequencePlayer;

	/*-------------------------
			Open Level
	-------------------------*/
	UFUNCTION(BlueprintCallable)
		void OpenLevelStart(FString Levelname, bool bPlaySequence, class APlayerControllerBase* PCBase = nullptr);

	void OpenLevelSepuenceEnd();

	UFUNCTION()
		void AsyncLodedMap();
	UFUNCTION()
		void LodeMap();
	FLevelPath LevelPath;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ULoadingScreenWidget> BP_LoadingScreen;
	UPROPERTY()
		class ULoadingScreenWidget* LoadingScreen;

	UPROPERTY(EditAnywhere)
		class ULevelSequence* OpenLevelSequence;

	bool bOpeningLevel = false;

	/*-----------------------------
			Sequence Actor
	------------------------------*/
	UFUNCTION(BlueprintCallable)
		class APlaySequenceActor* PlaySequence(int32 SequenceNumber, class APlayerControllerBase* Controller);


	/*-------------------
			Quest
	----------------------*/
	void SetLoadSlot(class UQuestComponent* QuestComponent);
	void SetSaveSlot(class UQuestComponent* QuestComponent);

	void SetNextQuest(int32 QuestNumber); // 게임 밖 다른 테마에서 퀘스트를 클리어 했을경우 인스턴스 안의 세이브파일에서 퀘스트 완료를 해준다.

	bool SetNPCLoadSlot(class AQuestNPCBase* NPC);
	void SetNPCSaveSlot(class AQuestNPCBase* NPC);
	void SetPlayerCanQuest();
	void AddCanQuest(int32 QuestNumber); // 가능한 퀘스트를 추가하는 함수.

	FNPCQuestDataBase* GetNPCQuestData(FString NPCName); // NPC 퀘스트를 가져온다.
	FQuestDataBase* GetPQData(int32 QuestNumber);
	TArray<FTextNName> GetDialData(EDialougeState DialState, int32 QuestNumber);
	FLevelPath* GetLevelPath(FString LevelName);
	FSequenceActorTable* GetSequenceActor(int32 SequenceNumber);

	void QuestClearNumber(FString NPCName, int32 QuestNumber); // ★★퀘스트 완료시 호출.

	/* 실시간으로 퀘스트를 추가하기. */
	UFUNCTION(BlueprintCallable)
		void AddQuestRunTime(FString NPCName, int32 QuestNumber, class AProjectH_PC* PlayerController);

	/* 다른 테마에서 메인레벨 퀘스트 완료 시키기 */
	UFUNCTION(BlueprintCallable)
		void SuccessQuestRunTime(int32 QuestNumber);
	UPROPERTY()
		TArray<int32> RunTimeSuccessQuestNumberQueue;

	/*-------------------
			Setting
	----------------------*/
	UPROPERTY()
		UGameUserSettings* UserSettings;
	bool SetDefault();
	void SetDefaultGameSetting();
	void GetDefaultGameSetting(int32* ResolutionIndex, int32* Anti, int32* ShadowQuality, int32* TextureQuality, float* MouseSensitivity, float* MasterSound);
	void GISetGameSetting(int32 ResolutionIndex, int32 Anti, int32 ShadowQuality, int32 TextureQuality, float MouseSensitivity, float MasterSound);
	/* 게임 인스턴스에 새로 설정된 셋팅 저장하면서 동시에 커맨드 실행.*/



public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UQuestSave* QuestSave; // 퀘스트 세이브파일. NPC의 퀘스트나 플레이어가 가지고있는 퀘스트 저장.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UPlayerStateSave* PlayerSave; // 캐릭터에 관한 세이브파일

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<FString, FCanQuestNums> PlayerCanQuest; // NPC와 비교할 것. 퀘스트 넘버로 데이터테이블에서 가져옴.

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int32> StartQuestNums; // ★★★ 실행 초기에 실행가능한 퀘스트를 미리 지정해둠.

	UPROPERTY(VisibleAnywhere)
		TArray<int32> QuestNums; // 진행 가능한 퀘스트 넘버로 가지고있다.

	UPROPERTY(VisibleAnywhere)
		TArray<int32> FinishedQuests; // 완료한 퀘스트들.


	/*-------------------
			DataTable
	----------------------*/

	UPROPERTY(EditAnywhere)
		class UDataTable* NPCQBTable; // NPC의 퀘스트들이 넣어져있는 테이블에서 NPC가 스스로 퀘스트 가져오기 위한 테이블.

	UPROPERTY(EditAnywhere)
		class UDataTable* PQTable; // 플레이어가 가능한 퀘스트들의 테이블

	UPROPERTY(EditAnywhere)
		class UDataTable* DialTable;

	UPROPERTY(EditAnywhere)
		class UDataTable* LevelPathTable;

	UPROPERTY(EditAnywhere)
		class UDataTable* SequenceActorTable;

	/*----------------------------------------------*/

	UPROPERTY(VisibleAnywhere)
		TMap<FString, class AQuestNPCBase*> NPCAllPtr; // NPC들을 이름으로 저장해둔다.
	void SetNPCPtr(FString Name, class AQuestNPCBase* NPC);
	// 해당 맵에 저장하는 함수,

	UFUNCTION(BlueprintCallable)
		class AQuestNPCBase* GetNPCPtr(FString NPCName);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> ResolutionArr; // 지원되는 해상도 저장.
	UPROPERTY()
		TArray<FIntPoint> ResArr;

	/* 맨 처음 해상도들을 저장한다. */
	UPROPERTY()
		int32 ResIndex;

	UPROPERTY()
		int32 AA;
	UPROPERTY()
		int32 S;
	UPROPERTY()
		int32 T;
	UPROPERTY()
		float MS;
	UPROPERTY()
		float MSound;


	/*---------------------------------------------------
		ARPG 클리어시 엔딩이 일어나게 할 것인가?
	---------------------------------------------------*/

	UFUNCTION(BlueprintCallable)
		void SetDontPlayEnding();
	UFUNCTION(BlueprintCallable)
		bool CanPlayEnding();
	bool bDontPlayEnding;
	
};
